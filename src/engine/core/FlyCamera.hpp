#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct GLFWwindow;

namespace lei3d
{
    class Application;

    class FlyCamera
    {
    private:
        const float MIN_FLY_SPEED = 1.0f;
        const float MAX_FLY_SPEED = 200.0f;
        const float MAX_PITCH  = 89.0f;
        const float MOUSE_SENSITIVITY = 0.1f;

        float m_Yaw;
        float m_Pitch;
        float m_FlySpeed;

        bool m_MouseEnterFlag;

        //projection values
        float m_FOVDeg, m_Aspect, m_NearPlane, m_FarPlane;

        int m_PrevX;
        int m_PrevY;

        glm::vec3 m_CameraPos;
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraUp;
    public:        
        FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed);
        ~FlyCamera();

        glm::mat4 GetView();
        glm::mat4 GetProj();
        glm::vec3 GetPosition();

        void cameraMouseCallback(double xPosInput, double yPosInput);
        void PollCameraMovementInput();

        void OnImGuiRender();

        void SetFOV(float fov);
        void SetClipPlanes(float near, float far);

    private:
        void handleForward(float speed);
        void handleLeft(float speed);
        void handleRight(float speed);
        void handleBack(float speed);
    };
}