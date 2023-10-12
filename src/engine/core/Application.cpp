#define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

#include "logging/GLDebug.hpp"

#include <stb_image.h>

namespace lei3d
{

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// TODO: Somehow change the engine window's width and height
		GLCall(glViewport(0, 0, width, height));
	}

	Application::Application()
	{
	}

	Application& Application::GetInstance()
	{
		static Application instance;
		return instance;
	}

	Application::~Application()
	{
		// Shutdown GLFW
		glfwDestroyWindow(m_Window);
		glfwTerminate();

		// Shutdown IMGUI
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Run()
	{
		LEI_TRACE("Initializing Engine");
		Initialize();

		LEI_ASSERT(m_ActiveScene != nullptr, "Please make sure a scene is set before running");

		LEI_TRACE("Entering Frame Loop");
		while (!glfwWindowShouldClose(m_Window))
		{
			// We need to block frame ticking until the new scene has been loaded
			// Not the most optimal scene loader (may need to look into async loading), but works for now.
			if (m_SceneManager->NeedsSceneSwitch())
			{
				m_SceneManager->LoadNextScene();
			}
			FrameTick();
		}

		LEI_TRACE("Gracefully Closing and Cleaning Up Data");
	}

	void Application::Initialize()
	{
		// GLFW INITIALIZE --------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

		GetMonitorConfiguration();

		m_Window = glfwCreateWindow(screenWidth, screenHeight, "lei3d", nullptr, nullptr);
		if (m_Window == nullptr)
		{
			LEI_WARN("failed to create GLFW window");
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LEI_WARN("failed to initialize GLAD");
			return;
		}

		GLCall(glViewport(0, 0, screenWidth, screenHeight));

		// resize the openGL context if a user changes the window size
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

		GLCall(glEnable(GL_DEPTH_TEST));

		// IMGUI SETUP --------------------------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		// Set ImGui Style
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init();

		// AppGUI --------------------------------
		m_EditorGUI = std::make_unique<EditorGUI>();
		SetUIActive(false);

		// CREATE SCENES --------------------------------
		m_SceneManager = std::make_unique<SceneManager>();
		m_SceneManager->Init();

		LEI_TRACE("Loading Default Scene");
		if (SceneManager::HasScenes())
		{
			m_SceneManager->SetScene(0);
		}
		else
		{
			LEI_WARN("NO SCENES SET (Check Build.config)");
		}

		// INIT SCENE VIEWER ------------------------------
		m_SceneView = std::make_unique<SceneView>();
		m_SceneView->SetMode(SceneView::ModeScene);

		// INIT AUDIO ENGINE ------------------------------
		m_AudioPlayer = std::make_unique<AudioPlayer>();

		// INIT RENDERER -----------------------------
		m_Renderer.initialize(screenWidth, screenHeight);
		m_PrimitiveRenderer.initialize(screenWidth, screenHeight);
		m_fontRenderer.Init();

		// INPUT CALLBACKS ------------------------------
		SetupInputCallbacks();
	}

	void Application::FrameTick()
	{
		// This is called every frame and controls the execution order of everything and frame syncing.

		const auto currentTime = static_cast<float>(glfwGetTime());

		m_DeltaTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;

		glfwPollEvents();
		Update();
		Render();
		ImGuiRender();

		glfwSwapBuffers(m_Window);

		// Sync FPS to desired value
		const float currentDrawTime = (float)glfwGetTime() - currentTime;
		const float sleepTime = 1.0f / m_DesiredFPS - currentDrawTime;

		typedef std::chrono::duration<int, std::milli> ms;

		const ms sleepDuration = ms((int)(sleepTime * 1000));
		if (sleepTime > 0.0f)
		{
			// Note: Need to fix, currently doesn't cap at the right fps.
			std::this_thread::sleep_for(sleepDuration);
			//_sleep(sleepTime);    //Platform Dependent
		}
	}

	void Application::Update()
	{
		Scene& scene = m_SceneManager->ActiveScene();
		scene.Update();
		scene.PhysicsUpdate();
		m_SceneView->Update(scene);
	}

	void Application::SetUIActive(bool uiActive)
	{
		m_UIActive = uiActive;
		int cursorMode = m_UIActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(m_Window, GLFW_CURSOR, cursorMode);
	}

	void Application::Render()
	{
		m_Renderer.draw(SceneManager::ActiveScene(), *m_SceneView);

		Camera& sceneCamera = m_SceneView->ActiveCamera(SceneManager::ActiveScene());
		m_PrimitiveRenderer.drawAll(sceneCamera);
	}

	void Application::ImGuiRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_UIActive)
		{
			m_EditorGUI->RenderUI();
		}

		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
	}

	SceneView& Application::GetSceneView()
	{
		return *m_SceneView;
	}

	PrimitiveRenderer& Application::GetPrimitiveRenderer()
	{
		return m_PrimitiveRenderer;
	}

	FontRenderer& Application::GetFontRenderer()
	{
		return m_fontRenderer;
	}

	// TODO: Put into input class
	void Application::SetupInputCallbacks()
	{
		glfwSetWindowUserPointer(m_Window, this);

		// NOTE: We need to be careful about overwriting the inputs, bc that will screw over imgui.
		// TODO: https://trello.com/c/S05x7OxG/33-input-callbacks
		// Create an input class where you can add and remove input callback functions from a list,
		// and call them here.

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
			int currentCursorMode = glfwGetInputMode(window, GLFW_CURSOR);
			const bool cursorDisabled = currentCursorMode == GLFW_CURSOR_DISABLED;

			if (!cursorDisabled)
			{
				ImGui_ImplGlfw_CursorPosCallback(window, x, y);
			}

			ImGuiIO& io = ImGui::GetIO();
			if (!io.WantCaptureMouse)
			{
				// Do mouse position things in game
				// Only pass data to the app if ImGui is not using it.

				Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
				if (self)
				{
					if (cursorDisabled)
					{
						Camera& sceneCamera = Application::GetInstance().GetSceneCamera();
						sceneCamera.cameraMouseCallback(x, y);
					}
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

			ImGuiIO& io = ImGui::GetIO();
			if (!io.WantCaptureMouse)
			{
				// Do Mouse Button things in game
			}
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

			ImGuiIO& io = ImGui::GetIO();
			if (!io.WantCaptureKeyboard)
			{
				// Do cursor position things in game
				Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
				if (self)
				{
					self->ProcessKeyboardInput(window, key, scancode, action, mods);
					self->m_SceneView->ProcessKeyboardInput(window, key, scancode, action, mods);
				}
			}
		});
	}

	void Application::ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// gracefully exit on escape
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// Editor Specific Controls
		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			SetUIActive(!m_UIActive);
		}
	}

	GLFWwindow* Application::Window()
	{
		return m_Window;
	}

	float Application::DeltaTime()
	{
		return m_DeltaTime;
	}

	void Application::GetMonitorConfiguration()
	{
		m_Monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);

		screenWidth = mode->width;
		screenHeight = mode->height;
	}

} // namespace lei3d
