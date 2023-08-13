#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.hpp"

#include "scenes/EmptyScene.hpp"
#include "scenes/TestSceneKevin.hpp"
#include "scenes/TestSceneLogan.hpp"

#include "logging/GLDebug.hpp"


namespace lei3d
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // TODO: Somehow change the engine window's width and height 
        GLCall(glViewport(0, 0, width, height));
    }

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        if (s_Instance) {
            LEI_ERROR("Multiple instances detected. Only one application should be running at a time.");
        }

        s_Instance = this;
    }

    Application::~Application()
    {
        s_Instance = nullptr;

        //if (groundPlane)
        //{
        //    GLCall(glDeleteBuffers(1, &(groundPlane->planeVAO)));
        //    GLCall(glDeleteBuffers(1, &(groundPlane->planeEBO)));
        //    GLCall(glDeleteBuffers(1, &(groundPlane->planeVBO)));
        //    delete groundPlane;
        //}

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
        Inititalize();

        LEI_ASSERT(m_ActiveScene != nullptr, "Please make sure a scene is set before running");

        LEI_TRACE("Entering Frame Loop");
        while (!glfwWindowShouldClose(m_Window))
        {
            //We need to block frame ticking until the new scene has been loaded
            //Not the most optimal scene loader (may need to look into async loading), but works for now.
            if (m_SceneChanged)
            {
                m_ActiveScene = m_NextScene;
                LoadScene(*m_ActiveScene);
                m_SceneChanged = false;
            }
            FrameTick();
        }

        LEI_TRACE("Gracefully Closing and Cleaning Up Data");
    }

    void Application::ChangeScenes(Scene& scene)
    {
        m_NextScene = &scene;
        m_SceneChanged = true;
    }

    void Application::Inititalize()
    {
        // GLFW INITIALIZE --------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        int screenWidth = 1200;
        int screenHeight = 1000;

        m_Window = glfwCreateWindow(screenWidth, screenHeight, "lei3d", NULL, NULL);
        if (m_Window == NULL)
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

        //IMGUI SETUP --------------------------------
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        //Set ImGui Style
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init();

        //AppGUI --------------------------------
        m_AppGUI = std::make_unique<AppGUI>();
        SetUIActive(false);

        //CREATE SCENES --------------------------------
        m_AllScenes.push_back({ "Test Kevin", std::make_unique<TestSceneKevin>() });
        m_AllScenes.push_back({ "Test Logan", std::make_unique<TestSceneLogan>() });
        m_AllScenes.push_back({ "Empty", std::make_unique<EmptyScene>() });

        LEI_TRACE("Loading Default Scene");
        Scene* defaultScene = m_AllScenes[0].second.get(); //This just gets the first scene we added
        ChangeScenes(*defaultScene);

        SetupInputCallbacks();
    }

    void Application::LoadScene(Scene& scene)
    {
        if (m_ActiveScene != nullptr)
        {
            m_ActiveScene->Unload();
        }

        m_ActiveScene = &scene;
        m_ActiveScene->Init(this);
    }

    void Application::FrameTick() {
        //This is called every frame and controls the execution order of everything and frame syncing.

        const float currentTime = (float)glfwGetTime();

        m_DeltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;

        glfwPollEvents();
        Update();
        Render();
        ImGuiRender();

        glfwSwapBuffers(m_Window);

        //Sync FPS to desired value
        const float currentDrawTime = (float)glfwGetTime() - currentTime;
        const float sleepTime = 1.0f / m_DesiredFPS - currentDrawTime;

        typedef std::chrono::duration<int, std::milli> ms;

        ms sleepDuration = ms((int) (sleepTime * 1000));
        if (sleepTime > 0.0f) {
            //Note: Need to fix, currently doesn't cap at the right fps.
            std::this_thread::sleep_for(sleepDuration);
            //_sleep(sleepTime);    //Platform Dependent
        }
    }

    void Application::Update() {
        m_ActiveScene->Update();
        m_ActiveScene->PhysicsUpdate();    //idk how often we need to do this.
    }

    void Application::SetUIActive(bool uiActive)
    {
        m_UIActive = uiActive;
        int cursorMode = m_UIActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
        glfwSetInputMode(m_Window, GLFW_CURSOR, cursorMode);
    }

    void Application::Render()
    {
        m_ActiveScene->Render();
    }

    void Application::ImGuiRender() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_UIActive)
        {
            m_AppGUI->RenderUI();
        }

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
        ImGui::EndFrame();
    }

    //TODO: Put into input class
    void Application::SetupInputCallbacks() {
        glfwSetWindowUserPointer(m_Window, this);

        //NOTE: We need to be careful about overwriting the inputs, bc that will screw over imgui.
        //TODO: https://trello.com/c/S05x7OxG/33-input-callbacks
        //Create an input class where you can add and remove input callback functions from a list,
        //and call them here.

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
            {
                int currentCursorMode = glfwGetInputMode(window, GLFW_CURSOR);
                const bool cursorDisabled = currentCursorMode == GLFW_CURSOR_DISABLED;

                if (!cursorDisabled) {
                    ImGui_ImplGlfw_CursorPosCallback(window, x, y);
                }

                ImGuiIO& io = ImGui::GetIO();
                if (!io.WantCaptureMouse) {
                    //Do mouse position things in game
                    //Only pass data to the app if ImGui is not using it.

                    Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
                    if (self)
                    {
                        if (cursorDisabled) {
                            self->m_ActiveScene->MainCamera().cameraMouseCallback(x, y);
                        }
                    }
                }

            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

            ImGuiIO& io = ImGui::GetIO();
            if (!io.WantCaptureMouse) {
                //Do Mouse Button things in game
            }
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

                ImGuiIO& io = ImGui::GetIO();
                if (!io.WantCaptureKeyboard) {
                    //Do cursor position things in game
                    Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
                    if (self)
                    {
                        self->ProcessKeyboardInput(window, key, scancode, action, mods);
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
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        {
            SetUIActive(!m_UIActive);
        }
    }

    Application& Application::Curr() {
        LEI_ASSERT(s_Instance != nullptr, "Application singleton not set. This shouldn't happen!");
        return *s_Instance;
    }

    GLFWwindow* Application::Window() const
    {
        return m_Window;
    }

    Scene& Application::ActiveScene() const {
        LEI_ASSERT(m_ActiveScene, "Attempt to access active scene when there wasn't one.");
        return *m_ActiveScene;
    }

    const std::vector<std::pair<std::string, std::unique_ptr<Scene>>>& Application::GetScenes() const
    {
        return m_AllScenes;
    }

    float Application::DeltaTime() const {
        return m_DeltaTime;
    }
}