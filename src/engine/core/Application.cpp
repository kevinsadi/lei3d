#define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

#include "logging/GLDebug.hpp"
#include "core/InputManager.hpp"
#include "core/SceneManager.hpp"

#include <stb_image.h>

#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/screens/MainMenuScreen.hpp"

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
			if (SceneManager::GetInstance().NeedsSceneSwitch())
			{
				SceneManager::GetInstance().LoadNextScene();
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

		// INPUT CALLBACKS ------------------------------
		// set glfw input callbacks before imgui, so it initializes properly for imgui as well
		InputManager::initialize(m_Window);

		// IMGUI SETUP --------------------------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		// Set ImGui Style
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// AppGUI --------------------------------
		m_EditorGUI = std::make_unique<EditorGUI>();
		SetIMGUIActive(false);

		// CREATE SCENES --------------------------------
		SceneManager& sm = SceneManager::GetInstance();
		sm.Init();

		LEI_TRACE("Loading Default Scene");
		if (SceneManager::HasScenes())
		{
			sm.SetScene(0);
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

		GuiManager::Instance().Init();
	}

	void Application::FrameTick()
	{
		// This is called every frame and controls the execution order of everything and frame syncing.

		const auto currentTime = static_cast<float>(glfwGetTime());

		m_DeltaTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;

		m_PhysicsElapsedTime += m_DeltaTime;
		if (m_PhysicsElapsedTime >= m_FixedDeltaTime)
		{
			FixedUpdate();
			m_PhysicsElapsedTime -= m_FixedDeltaTime;
		}

		Update();
		Render();
		ImGuiRender();

		glfwPollEvents();
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
		Scene& scene = SceneManager::GetInstance().ActiveScene();
		scene.Update();

		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse || io.WantCaptureKeyboard)
		{
			// nothing
		}
		else
		{
			ProcessInput();
			m_SceneView->Update(scene);
		}
	}

	void Application::FixedUpdate()
	{
		Scene& scene = SceneManager::GetInstance().ActiveScene();
		scene.PhysicsUpdate();
	}

	void Application::SetIMGUIActive(bool uiActive)
	{
		m_ImGUIActive = uiActive;

		if (uiActive)
		{
			InputManager::GetInstance().giveInputFocus(InputManager::InputTarget::IMGUI);
		}
		else
		{
			InputManager::GetInstance().giveInputFocus(InputManager::InputTarget::GAME);
		}
	}

	void Application::Render()
	{
		m_Renderer.draw(SceneManager::ActiveScene(), *m_SceneView);

		Camera& sceneCamera = m_SceneView->ActiveCamera(SceneManager::ActiveScene());
		// Maybe enable this in debug mode only?
		// if (m_UIActive)
		// {
		 	EditorGUI::renderDebugCursor();
		// }
		m_PrimitiveRenderer.drawAll(sceneCamera);
		// SceneManager::ActiveScene().GetPhysicsWorld().m_dynamicsWorld->debugDrawWorld();
	}

	void Application::ImGuiRender()
	{
		if (m_ImGUIActive)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			m_EditorGUI->RenderUI();

			ImGui::Render();
			ImDrawData* drawData = ImGui::GetDrawData();
			ImGui_ImplOpenGL3_RenderDrawData(drawData);
		}
	}

	SceneView& Application::GetSceneView()
	{
		return *m_SceneView;
	}

	PrimitiveRenderer& Application::GetPrimitiveRenderer()
	{
		return m_PrimitiveRenderer;
	}

	void Application::ProcessInput()
	{
		InputManager& im = InputManager::GetInstance();
		im.update(m_Window);

		Camera& sceneCamera = GetInstance().GetSceneCamera();
		sceneCamera.Pan();

		// gracefully exit on escape
		if (im.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(m_Window, true);
		}

		// open sample splash screen
		if (im.isKeyPressed(GLFW_KEY_0))
		{
			GuiManager::Instance().QueueNextScreen(new MainMenuScreen());
		}

		// Editor Specific Controls
		if (im.isKeyPressed(GLFW_KEY_TAB, m_ImGUIActive ? InputManager::InputTarget::IMGUI : InputManager::InputTarget::GAME))
		{
			SetIMGUIActive(!m_ImGUIActive);
		}
	}

	GLFWwindow* Application::Window()
	{
		return GetInstance().m_Window;
	}

	float Application::DeltaTime()
	{
		return GetInstance().m_DeltaTime;
	}

	void Application::GetMonitorConfiguration()
	{
		m_Monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);

		screenWidth = mode->width;
		screenHeight = mode->height;
	}

} // namespace lei3d
