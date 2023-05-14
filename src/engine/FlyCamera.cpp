#include "include/engine/FlyCamera.hpp"
#include <iostream>

namespace kek3d
{
    FlyCamera::FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed)
    {
        this->window = window;
        this->yaw = yaw;
        this->pitch = pitch;
        this->flySpeed = flySpeed;
        this->firstMouse = true;

        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        std::cout << screenWidth << "a     " << screenHeight << std::endl;

        lastX = screenWidth / 2.0f;  
        lastY = screenHeight / 2.0f; 

        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    FlyCamera::~FlyCamera()
    {
        // no need rn
    }


    void FlyCamera::cameraMouseCallback(GLFWwindow* window, double xPosInput, double yPosInput)
    {
        float xpos = static_cast<float>(xPosInput);
        float ypos = static_cast<float>(yPosInput);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // openGL inverted y
        lastX = xpos;
        lastY = ypos;

        const float mouseSensitivity = 0.1f;
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction) * flySpeed;
    }

    void FlyCamera::handleForward()
    {
		cameraPos += cameraFront * flySpeed;
    }

    void FlyCamera::handleBack()
    {
		cameraPos -= cameraFront * flySpeed;
    }

    void FlyCamera::handleLeft()
    {
		cameraPos -= glm::cross(cameraFront, cameraUp) * flySpeed;
    }

    void FlyCamera::handleRight()
    {
		cameraPos += glm::cross(cameraFront, cameraUp) * flySpeed;
    }

    glm::mat4 FlyCamera::getCameraView()
    {
        cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        return cameraView;
    }

}