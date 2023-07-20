#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.hpp"

#include "scenes/TestScene.hpp"
#include "util/GLDebug.hpp"

namespace lei3d
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // TODO: Somehow change the engine window's width and height 
        GLCall(glViewport(0, 0, width, height));
    }

    Application::Application()
    {
        // clown emoji
        // kevin please fix
    }

    Application::~Application()
    {
        if (groundPlane)
        {
            GLCall(glDeleteBuffers(1, &(groundPlane->planeVAO)));
            GLCall(glDeleteBuffers(1, &(groundPlane->planeEBO)));
            GLCall(glDeleteBuffers(1, &(groundPlane->planeVBO)));
            delete groundPlane;
        }

        // add clean up for the skybox buffers, especially if we switch the buffers

        // clear all of the previously allocated GLFW resources
        glfwTerminate();

        // turn off imgui resources 
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::Run()
    {
        LEI_ASSERT(m_ActiveScene != nullptr, "Please make sure a scene is set before running");

        LEI_TRACE("Initializing Engine");
        Inititalize();
        
        LEI_TRACE("Loading Scene");
        Scene* testScene = new TestScene();
        LoadScene(testScene);

        LEI_TRACE("Entering Frame Loop");
        while (!glfwWindowShouldClose(m_Window))
        {
            FrameTick();
        }

        LEI_TRACE("Gracefully Closing and Cleaning Up Data");
        delete testScene;
    }

    GLFWwindow* Application::Window()
    {
        return m_Window;
    }

    void Application::Inititalize()
    {
        // *** initialize window ***
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

        // set up imgui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init();
    }

    void Application::LoadScene(Scene* scene)
    {
        GLCall(glEnable(GL_DEPTH_TEST));

        m_ActiveScene = scene;
        m_ActiveScene->Init(this);
        SetupSceneCallbacks();
    }

    void Application::SetupSceneCallbacks() {
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn off if UI
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
            {
                Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
                if (self)
                {
                    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) // when the cursor is normal, don't move the camera according to mouse mvnt
                        self->m_ActiveScene->MainCamera()->cameraMouseCallback(window, x, y);
                }
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                Application* self = static_cast<Application*>(glfwGetWindowUserPointer(window));
                if (self)
                {
                    self->processInput(window, key, scancode, action, mods);
                }
            });
    }

    void Application::FrameTick() {
        const float currentTime = (float)glfwGetTime();

        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        glfwPollEvents();
        Update();
        Render();

        m_ActiveScene->ProcessCameraInput(deltaTime);
        glfwSwapBuffers(m_Window);

        //Sync FPS to desired value
        const float currentDrawTime = (float)glfwGetTime() - currentTime;
        const float sleepTime = 1.0f / desiredFPS - currentDrawTime;
        if (sleepTime > 0.0f) {
            _sleep(sleepTime);
        }
    }

    void Application::Update() {
        m_ActiveScene->Update(deltaTime);
        m_ActiveScene->PhysicsUpdate(deltaTime);    //idk how often we need to do this.
    }

    void Application::Render()
    {
        m_ActiveScene->Render();

        // Render UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            RenderUI(deltaTime);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            ImGui::EndFrame();
        }
    }

    void Application::RenderUI(float deltaTime)
    {
        ImGui::Begin("Debug Window");
        ImGui::Text("PRESS TAB TO UNLOCK MOUSE");
        ImGui::Text("fps = %f", 1.0f / deltaTime);
        ImGui::SetWindowSize(ImVec2(0, 0));
        ImGui::End();
    }

    void Application::processInput(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // gracefully exit on escape
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        {
            int currentCursorMode = glfwGetInputMode(window, GLFW_CURSOR);
            int reverseCursorLockMode = (currentCursorMode == GLFW_CURSOR_DISABLED) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
            glfwSetInputMode(window, GLFW_CURSOR, reverseCursorLockMode); // turn off if UI
        }
    }
}