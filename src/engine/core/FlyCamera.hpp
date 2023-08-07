#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "core/Application.hpp"

namespace lei3d
{
    class Application;

    class FlyCamera
    {
    public:        
        FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed);
        ~FlyCamera();

        glm::mat4 GetView();
        glm::mat4 GetProj();

        void SetFOV(float fov);
        void SetClipPlanes(float near, float far);

        void cameraMouseCallback(GLFWwindow* window, double xPosInput, double yPosInput);
        void PollCameraMovementInput(float deltaTime);
        
        void handleForward(float deltaTime, float speed);
        void handleLeft(float deltaTime, float speed);
        void handleRight(float deltaTime, float speed);
        void handleBack(float deltaTime, float speed);

        void OnImGuiRender();

    private:
        const float minFlySpeed = 1.0f;
        const float maxFlySpeed = 200.0f;

        GLFWwindow* window;
        float yaw;
        float pitch;
        float flySpeed;
        bool firstMouse;

        //projection values
        float m_FOVDeg, m_Aspect, m_NearPlane, m_FarPlane;

        glm::mat4 cameraView;

        int lastX;
        int lastY;

        glm::vec3 cameraFront;
        glm::vec3 cameraPos;
        glm::vec3 cameraUp;

    };
}