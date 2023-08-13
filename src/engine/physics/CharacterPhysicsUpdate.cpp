#include "CharacterPhysicsUpdate.hpp"

namespace lei3d 
{
    CharacterPhysicsUpdate::CharacterPhysicsUpdate(btRigidBody* body) : m_body(body) {}

    void CharacterPhysicsUpdate::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
    {
    

        btVector3 velocity = m_body->getLinearVelocity();

        // Calculate 
        //glm::vec3 wo{0.0f, 0.0f, 0.0f};

        // here is where we apply our constraints during the update
        GLFWwindow* window = Application::Curr().Window();
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            velocity = velocity + btVector3(-0.5, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            velocity = velocity + btVector3(0.5, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            velocity = velocity + btVector3(0.0, 0.0, 0.5);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            velocity = velocity + btVector3(0.0, 0.0, -0.5);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            velocity = velocity + btVector3(0.0, 0.0, 0.5);
        }

        m_body->setLinearVelocity(velocity);
    }

    void CharacterPhysicsUpdate::debugDraw(btIDebugDraw* debugDrawer) {}

}
