#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "include/engine/Engine.hpp"

namespace lei3d
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // TODO: Somehow change the engine window's width and height 
        glViewport(0, 0, width, height);
    }

    Engine::Engine()
    {
        // clown emoji
        // kevin please fix
    }

    Engine::~Engine()
    {
        if (groundPlane)
        {
            glDeleteBuffers(1, &(groundPlane->planeVAO));
            glDeleteBuffers(1, &(groundPlane->planeEBO));
            glDeleteBuffers(1, &(groundPlane->planeVBO));
            delete groundPlane;
        }

        // add clean up for the skybox buffers, especially if we switch the buffers

        if (camera)
          delete camera;

        if (meshModel)
            delete meshModel;

        // clear all of the previously allocated GLFW resources
        glfwTerminate();

        // turn off imgui resources 
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Engine::Start()
    {
        std::cout << "Initializing Engine" << std::endl;
        Inititalize();
        std::cout << "Loading Resources" << std::endl;
        Load();

        std::cout << "Entering Rendering Loop" << std::endl;
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            Render();
        }
        std::cout << "Gracefully Closing and Cleaning Up Data" << std::endl;
    }

    void Engine::Inititalize()
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

        window = glfwCreateWindow(screenWidth, screenHeight, "lei3d", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "failed to initialize GLAD" << std::endl;
            return;
        }

        glViewport(0, 0, screenWidth, screenHeight);

        // resize the openGL context if a user changes the window size
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // set up imgui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        // Create physics world
        physicsObjects = CreatePhysicsWorld();
    }

    void Engine::Load()
    {
    	glEnable(GL_DEPTH_TEST);

        // load shaders
        shader = Shader("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // load mesh from obj file (EVENTUALLY WILL WANT TO USE GTLF FILES INSTEAD)
        std::string path = "data/models/backpack/backpack.obj";

        this->meshModel = new Model(path);

        // load camera
        this->camera = new FlyCamera(window, 90.0f, 0.0f, 4.0f);
	    glfwSetWindowUserPointer(window, this);

        // set glfw inputs
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn off if UI
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
        {
            Engine* self = static_cast<Engine*>(glfwGetWindowUserPointer(window));
            if (self)
            {
                if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) // when the cursor is normal, don't move the camera according to mouse mvnt
                    self->camera->cameraMouseCallback(window, x, y);
            }
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Engine* self = static_cast<Engine*>(glfwGetWindowUserPointer(window));
            if (self)
            {
                self->processInput(window, key, scancode, action, mods);
            }
        });

        // create skybox
        SkyBox skybox = SkyBox();

        std::vector<std::string> faces
        {
            "data/skybox/anime_etheria/right.jpg",
            "data/skybox/anime_etheria/left.jpg",
            "data/skybox/anime_etheria/up.jpg",
            "data/skybox/anime_etheria/down.jpg",
            "data/skybox/anime_etheria/front.jpg",
            "data/skybox/anime_etheria/back.jpg"
        };
        skybox.loadCubemap(faces);

        skybox.skyboxShader.use();
        skybox.skyboxShader.setInt("skybox", 0);
        this->skybox = skybox;
    }

    void Engine::Render()
    {
        float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        PhysicsStep(this->physicsObjects);
        RenderScene();
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
        processCameraInput(deltaTime);
        glfwSwapBuffers(window);
    }

    void Engine::RenderUI(float deltaTime)
    {
        ImGui::Begin("Window");        
        ImGui::Text("fps = %f", 1/deltaTime);
        ImGui::SetWindowSize(ImVec2(0, 0));
        ImGui::End();
    }

    void Engine::RenderScene()
    {
        // rendering
		glClearColor(0.2f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera shader 
		shader.use();

        // -- Set up camera views and pass to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
		shader.setUniformMat4(projection, "proj");
		
		glm::mat4 view = camera->getCameraView();
		shader.setUniformMat4(view, "view");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
		model = glm::translate(model, glm::vec3(-64, 0, -64));
		shader.setUniformMat4(model, "model");

        // draw mesh
        meshModel->Draw(shader);

        // render skybox after rendering rest of the scene (only draw skybox where an object is not present)
        glDepthFunc(GL_LEQUAL); // we change the depth function here to it passes when testingdepth value is equal to what is current stored
        skybox.skyboxShader.use();
        view = glm::mat4(glm::mat3(camera->getCameraView()));
        skybox.skyboxShader.setUniformMat4(view, "view");
        skybox.skyboxShader.setUniformMat4(projection, "projection");
        // -- render the skybox cube
        glBindVertexArray(skybox.skyboxVAO);
        glActiveTexture(GL_TEXTURE0); //! could be the problem
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubeMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to normal
    }

    void Engine::processInput(GLFWwindow* window, int key, int scancode, int action, int mods)
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

    void Engine::processCameraInput(float deltaTime)
    {
        if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            this->camera->handleForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            this->camera->handleBack(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            this->camera->handleLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            this->camera->handleRight(deltaTime);
        }
    }

}
