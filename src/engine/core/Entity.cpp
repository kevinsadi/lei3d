#include "Entity.hpp"

#include "logging/Log.hpp"
#include "logging/LogGLM.hpp"

#include <imgui.h>

namespace lei3d
{
	Entity::Entity()
	{
		m_Name = "Unnamed";
	}

	Entity::Entity(const std::string& name)
	{
		m_Name = name;
	}

	Entity::~Entity()
	{
		OnDestroy();
	}

	void Entity::SetPosition(const glm::vec3& position)
	{
		m_Transform.position = position;
	}

	void Entity::SetScale(const glm::vec3& scale)
	{
		m_Transform.scale = scale;
	}

	glm::mat4 Entity::GetTranslationMat()
	{
		return glm::translate(glm::identity<glm::mat4>(), m_Transform.position);
	}

	glm::mat4 Entity::GetRotationMat()
	{
		// TODO: Implement quaternion.
		return glm::identity<glm::mat4>();
	}

	glm::mat4 Entity::GetScaleMat()
	{
		return glm::scale(glm::identity<glm::mat4>(), m_Transform.scale);
	}

	const std::string& Entity::GetName()
	{
		return m_Name;
	}

	void Entity::SetName(const std::string& name)
	{
		m_Name = name;
	}

	glm::mat4 Entity::GetModelMat()
	{
		glm::mat4 translate = GetTranslationMat();
		glm::mat4 rotate = GetRotationMat();
		glm::mat4 scale = GetScaleMat();
		glm::mat4 model = translate * rotate * scale;
		// LEI_INFO("TRANSLATE:");
		// PrintMat4(translate);
		// LEI_INFO("ROTATE:");
		// PrintMat4(rotate);
		// LEI_INFO("SCALE:");
		// PrintMat4(scale);
		// LEI_INFO("MODEL: ");
		// PrintMat4(model);
		return model;
	}

	void Entity::Start()
	{
		LEI_TRACE("Started Entity");

		for (auto& component : m_Components)
		{
			component->Start();
		}
	}

	void Entity::Update()
	{
		for (auto& component : m_Components)
		{
			component->Update();
		}
	}

	void Entity::PhysicsUpdate()
	{
		for (auto& component : m_Components)
		{
			component->PhysicsUpdate();
		}
	}

	void Entity::Render()
	{
		for (auto& component : m_Components)
		{
			component->Render();
		}
	}

	void Entity::OnDestroy()
	{
		for (auto& component : m_Components)
		{
			component->OnDestroy();
		}
	}

	void Entity::ShowInspectorGUI()
	{
		ImGui::Begin("Inspector");

		// NAME
		const int maxNameSize = 100;
		const int currLength = m_Name.length();
		char	  buffer[maxNameSize + 1] = {};

		m_Name.copy(buffer, currLength);
		m_Name[currLength] = '\0';
		if (ImGui::InputText("Name", &buffer[0], maxNameSize))
		{
			std::string newName = std::string(buffer);
			if (newName.length() > maxNameSize)
			{
				newName = newName.substr(0, maxNameSize);
			}
			if (newName.length() == 0)
			{
				newName = "";
			}
			SetName(newName);
		}

		// TRANSFORM
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			float x = m_Transform.position.x;
			float y = m_Transform.position.y;
			float z = m_Transform.position.z;

			const float stepFine = 0.5f;
			const float stepFast = 10.0f; // Hold down Ctrl to scroll faster.

			ImGui::InputFloat("x", &x, stepFine, stepFast);
			// ImGui::SameLine();
			ImGui::InputFloat("y", &y, stepFine, stepFast);
			// ImGui::SameLine();
			ImGui::InputFloat("z", &z, stepFine, stepFast);

			// NOTE: If the position is not changing, it's probably because the physics engine (or something else) is overwriting it)
			SetPosition(glm::vec3(x, y, z));
		}

		ImGui::SetWindowSize(ImVec2(300, 800), ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(300, 0), ImGuiCond_Once);
		ImGui::End();
	}
} // namespace lei3d