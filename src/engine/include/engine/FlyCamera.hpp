#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace kek3d
{
    class FlyCamera
    {
    public:        
        FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed);
        ~FlyCamera();

        glm::mat4 getCameraView();
        void cameraMouseCallback(GLFWwindow* window, double xPosInput, double yPosInput);
        
        void handleForward();
        void handleLeft();
        void handleRight();
        void handleBack();

    private:
        GLFWwindow* window;
        float yaw;
        float pitch;
        float flySpeed;
        bool firstMouse;

        glm::mat4 cameraView;

        int lastX;
        int lastY;

        glm::vec3 cameraFront;
        glm::vec3 cameraPos;
        glm::vec3 cameraUp;

    };
}