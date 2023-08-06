#include "Component.hpp"

#include "core/Application.hpp"

namespace lei3d {
	Component::Component(Entity* entity) : m_Entity(entity) {

	}

	Scene& Component::ActiveScene() {
		Scene* scene = Application::Curr()->ActiveScene();
		LEI_ASSERT(scene, "Attempt to access active scene when there wasn't one.");
		return *scene;
	}
}