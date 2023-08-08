#include "CharacterPhysicsUpdate.hpp"

namespace lei3d 
{
    CharacterPhysicsUpdate::CharacterPhysicsUpdate(btRigidBody* body) : m_body(body) {}


    glm::vec3 projectVector(glm::vec3& vecToProject, glm::vec3& targetVector)
    {
        float dotProduct = glm::dot(vecToProject, targetVector);
        float targetLengthSquared = glm::length(targetVector) * glm::length(targetVector);
        return targetVector * (dotProduct / targetLengthSquared);
    }
    
    /**
     * CharacterPhysicsUpdate -> this is the airAccelerate function that allows airstrafing to work.
     * wishdir is the direction that the player presses on the keyboard to indicate the direction they would like to move.
     * 
     * @param  {btCollisionWorld*} collisionWorld : 
     * @param  {btScalar} deltaTime               : 
     */
    void CharacterPhysicsUpdate::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
    {
    
        btVector3 v = m_body->getLinearVelocity();
        glm::vec3 prevVel = glm::vec3(v.x(), v.y(), v.z());

        // Calculate 
        glm::vec3 wishdir{0.0f, 0.0f, 0.0f};

        // here is where we apply our constraints during the update
        GLFWwindow* window = Application::Curr()->Window();
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            wishdir = wishdir + glm::vec3(1.0, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            wishdir = wishdir + glm::vec3(-1.0, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            wishdir = wishdir + glm::vec3(0.0, 0.0, -1.0);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            wishdir = wishdir + glm::vec3(0.0, 0.0, 1.0);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            v = v + btVector3(0.0, 4.75, 0.0);
        }

        wishdir = glm::normalize(wishdir);
        
        
        // project current velocity onto the direction we wish to move
        glm::vec3 projVel = projectVector(prevVel, wishdir);

        // check if the wish direction is towards or away from the projected velocity
        bool isAway = glm::dot(wishdir, projVel) <= 0;
        float projectedVectorMagnitude = glm::length(projVel);

        // only apply force (add speed) if moving away from velocity or velocity is below maxairspeed
        if (projectedVectorMagnitude < m_maxAirSpeed || isAway)
        {
            // calculate the ideal movement force
            glm::vec3 vc = wishdir * m_airAccel; // NEEDS WISHDIR TO BE NORMALIZED
            float vcMagnitude = glm::length(vc);
            // cap the velocity if it would accelerate beyond maxairspeed in direction of velocity
            if (!isAway) {
                if (vcMagnitude > m_maxAirSpeed - projectedVectorMagnitude) 
                    vc = (vc / vcMagnitude) * m_maxAirSpeed - projectedVectorMagnitude;
            } else {
                if (vcMagnitude > m_maxAirSpeed + projectedVectorMagnitude) 
                    vc = (vc / vcMagnitude) * m_maxAirSpeed + projectedVectorMagnitude;
            }

            v = btVector3(vc.x, vc.y, vc.z);
        }

        // If necessary, truncate the accelerated velocity so the vector projection doesn't exceed max_velocity

        m_body->setLinearVelocity(v);
    }

    void CharacterPhysicsUpdate::debugDraw(btIDebugDraw* debugDrawer) {}
}