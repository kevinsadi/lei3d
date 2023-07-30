#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace lei3d
{
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
        
        void handleForward(float deltaTime);
        void handleLeft(float deltaTime);
        void handleRight(float deltaTime);
        void handleBack(float deltaTime);

    private:
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