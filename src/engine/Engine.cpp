#include "include/engine/Engine.hpp"
#include <iostream>

namespace kek3d
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // TODO: Somehow change the engine window's width and height 
        glViewport(0, 0, width, height);
    }

    Engine::Engine()
    {
        // clown emoji
        // kevin pls fix this
        shader = Shader("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");
    }

    Engine::~Engine()
    {
        glDeleteBuffers(1, &groundPlane.planeVAO);
        glDeleteBuffers(1, &groundPlane.planeEBO);
        glDeleteBuffers(1, &groundPlane.planeVBO);

        delete camera;

        // clear all of the previously allocated GLFW resources
        glfwTerminate();
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

        window = glfwCreateWindow(screenWidth, screenHeight, "kek3d", NULL, NULL);
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
    }

    void Engine::Load()
    {
    	glEnable(GL_DEPTH_TEST);

        // load shaders
        shader = Shader("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        // load textures
        stbi_set_flip_vertically_on_load(true);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set texture wrapping options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // load and generate texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("./data/textures/rgb.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "ERROR: Texture loading failed" << std::endl;
        }
        glUniform1i(glGetUniformLocation(shader.shaderProgramID, "texture1"), 0);
        stbi_image_free(data); // no memory leaks here, no sir

        // load pcg mesh
        groundPlane = createPlaneMesh();

        // load camera
        FlyCamera* camera = new FlyCamera(window, -90.0f, 0.0f, 1.2f);
	    glfwSetWindowUserPointer(window, camera);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // turn off if UI
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
        {
            if (FlyCamera* camera = static_cast<FlyCamera*>(glfwGetWindowUserPointer(window)))
            {
                camera->cameraMouseCallback(window, x, y);
            }
        });


    }

    void Engine::Render()
    {
        RenderScene();
        // Here is where I would render the UI
        glfwSwapBuffers(window);
    }

    void Engine::RenderScene()
    {
        float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

        // rendering
		glClearColor(0.2, 0.8, 0.9, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// input
		processInput(window, camera);

        // shaders 
		shader.use();

        // set up camera views and pass to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
		shader.setUniformMat4(projection, "proj");
		
		glm::mat4 view = camera->getCameraView();
		shader.setUniformMat4(view, "view");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
		model = glm::translate(model, glm::vec3(-64, 0, -64));
		shader.setUniformMat4(model, "model");

		// draw plane
        int dim = 128;
		glBindVertexArray(groundPlane.planeVAO);
		glDrawElements(GL_TRIANGLES, ((dim-1)*(dim-1)*6), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // unbind vertex array
    }

    void Engine::processInput(GLFWwindow* window, FlyCamera* camera)
    {
        const float cameraSpeed = 7.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera->handleForward();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera->handleBack();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera->handleLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera->handleRight();
        }
        // gracefully exit on escape
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

}
