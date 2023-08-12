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
        // Check if we are on the ground
        FindGround callback(m_body);
        collisionWorld->contactTest(m_body, callback);
        bool onGround = callback.m_Grounded;
        bool groundPoint = callback.m_GroundPoint;

        // Update velocity accordingly
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

        if (wishdir != glm::vec3(0.0, 0.0, 0.0)) {
            wishdir = glm::normalize(wishdir);
        }
        //std::cout << "wishdir is: " << wishdir.x << " " << wishdir.y << " " << wishdir.z << std::endl;

        if (onGround) {
            glm::vec3 outputVel = GroundAcceleration(wishdir, prevVel);
            v = btVector3(outputVel.x, outputVel.y, outputVel.z);
        } else {
            glm::vec3 outputVel = AirAcceleration(wishdir, prevVel);
            v = btVector3(outputVel.x, outputVel.y, outputVel.z);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
        {
            v = v + btVector3(0.0, 4.75, 0.0);
        }

        m_body->setLinearVelocity(v);
    }

    void CharacterPhysicsUpdate::debugDraw(btIDebugDraw* debugDrawer) {}

    glm::vec3 CharacterPhysicsUpdate::Accelerate(glm::vec3 wishDir, glm::vec3 prevVel, float acceleration, float maxVelocity)
    {
        glm::vec3 outputVelocity = prevVel;

        // project current velocity onto the direction we wish to move
        glm::vec3 projVel = projectVector(prevVel, wishDir);

        // check if the wish direction is towards or away from the projected velocity
        bool isAway = glm::dot(wishDir, projVel) <= 0;
        float projectedVectorMagnitude = glm::length(projVel);

        // only apply force (add speed) if moving away from velocity or velocity is below maxairspeed
        if (projectedVectorMagnitude < maxVelocity || isAway)
        {
            // calculate the ideal movement force
            glm::vec3 vc = wishDir * acceleration; // NEEDS WISHDIR TO BE NORMALIZED
            float vcMagnitude = glm::length(vc);
            // cap the velocity if it would accelerate beyond maxairspeed in direction of velocity
            if (!isAway) {
                if (vcMagnitude > maxVelocity - projectedVectorMagnitude) 
                    vc = (vc / vcMagnitude) * maxVelocity - projectedVectorMagnitude;
            } else {
                if (vcMagnitude > maxVelocity + projectedVectorMagnitude) 
                    vc = (vc / vcMagnitude) * maxVelocity + projectedVectorMagnitude;
            }
            outputVelocity = vc;
        }
        return outputVelocity;
    }

    glm::vec3 CharacterPhysicsUpdate::AirAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity)
    {
        return Accelerate(wishDir, prevVelocity, m_airAccel, m_maxAirSpeed);
    }

    glm::vec3 CharacterPhysicsUpdate::GroundAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity)
    {
        float speed = prevVelocity.length();
        if (speed != 0)
        {
            float drop = speed * m_friction; // THIS MIGHT HAVE TO BE MULTIPLIED BY DELTA TIME??
            prevVelocity *= std::max(speed - drop, 0.0f) / speed; // Friction fall off
        }

        return Accelerate(wishDir, prevVelocity, m_accel, m_maxSpeed);
    }
}