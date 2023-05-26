#include "include/engine/FlyCamera.hpp"

namespace kek3d
{
    FlyCamera::FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed)
    {
      this->window = window; // do I need to do this-> ?? I don't do it elsewhere but it's throwing bugs if I don't rn
      this->yaw = yaw;
      this->pitch = pitch;
      this->flySpeed = flySpeed;
      this->firstMouse = true;

      int screenWidth, screenHeight;
      glfwGetWindowSize(window, &screenWidth, &screenHeight);

      lastX = screenWidth / 2.0f;  
      lastY = screenHeight / 2.0f; 

      this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
      this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
      this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
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

    void FlyCamera::handleForward(float deltaTime)
    {
		  cameraPos += cameraFront * flySpeed * deltaTime;
    }

    void FlyCamera::handleBack(float deltaTime)
    {
		  cameraPos -= cameraFront * flySpeed * deltaTime;
    }

    void FlyCamera::handleLeft(float deltaTime)
    {
		  cameraPos -= glm::cross(cameraFront, cameraUp) * flySpeed * deltaTime;
    }

    void FlyCamera::handleRight(float deltaTime)
    {
		  cameraPos += glm::cross(cameraFront, cameraUp) * flySpeed * deltaTime;
    }

    glm::mat4 FlyCamera::getCameraView()
    {
      cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      return cameraView;
    }

}