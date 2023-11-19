#pragma once
#include "core/Component.hpp"
#include "physics/PlayerTriggerCallback.hpp"
#include <btBulletDynamicsCommon.h>

namespace lei3d {
    class PlayerTriggerComponent : public Component {
        public:
            PlayerTriggerComponent(Entity& entity);
            ~PlayerTriggerComponent();
            
            void SetCollider(btCollisionObject* collider);

            void SetOnPlayerEntered(const std::function<void()>& onPlayerEntered);
            void SetOnPlayerStayed(const std::function<void()>& onPlayerStayed);
            void SetOnPlayerExited(const std::function<void()>& onPlayerExited);

        private:
            btCollisionObject* m_collider;
			PlayerTriggerCallback m_callback;

    };
} // namespace lei3d
