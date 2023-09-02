#include "Entity.hpp"

#include "logging/Log.hpp"
#include "logging/LogGLM.hpp"

#include "util/BulletUtil.hpp"

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

	void Entity::OnEditorUpdate()
	{
		for (auto& component : m_Components)
		{
			component->OnEditorUpdate();
		}
	}

	const std::string& Entity::GetName() const
	{
		return m_Name;
	}

	void Entity::SetName(const std::string& name)
	{
		m_Name = name;
	}

	void Entity::SetPosition(const glm::vec3& position)
	{
		m_Transform.position = position;
	}

	void Entity::SetScale(const glm::vec3& scale)
	{
		m_Transform.scale = scale;
	}

	void Entity::SetYawRotation(const float yawRotation)
	{
		m_Transform.yawRotation = yawRotation;
	}

	glm::mat4 Entity::GetTranslationMat() const
	{
		return glm::translate(glm::identity<glm::mat4>(), m_Transform.position);
	}

	glm::mat4 Entity::GetRotationMat() const
	{
		//TODO: Implement quaternion.
		// we currently only support axis angle rotation around the y axis
		glm::mat4 potentialMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Transform.yawRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		potentialMatrix = glm::identity<glm::mat4>();
		potentialMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		potentialMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(m_Transform.yawRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		return potentialMatrix;
	}

	glm::mat4 Entity::GetScaleMat() const
	{
		return glm::scale(glm::identity<glm::mat4>(), m_Transform.scale);
	}

	glm::mat4 Entity::GetModelMat() const
	{
		glm::mat4 translate = GetTranslationMat();
		glm::mat4 rotate = GetRotationMat();
		glm::mat4 scale = GetScaleMat();
		glm::mat4 model = translate * rotate * scale;
		//LEI_INFO("TRANSLATE:");
		//PrintMat4(translate);
		//LEI_INFO("ROTATE:");
		//PrintMat4(rotate);
		//LEI_INFO("SCALE:");
		//PrintMat4(scale);
		//LEI_INFO("MODEL: ");
		//PrintMat4(model);
		return model;
	}

	void Entity::NameGUI()
	{
		constexpr int MAX_NAME_SIZE = 100;
		const int	  currLength = m_Name.length();
		char		  buffer[MAX_NAME_SIZE + 1] = {};

		m_Name.copy(buffer, currLength);
		m_Name[currLength] = '\0';
		if (ImGui::InputText("Name", &buffer[0], MAX_NAME_SIZE))
		{
			std::string newName = std::string(buffer);
			if (newName.length() > MAX_NAME_SIZE)
			{
				newName = newName.substr(0, MAX_NAME_SIZE);
			}
			if (newName.length() == 0)
			{
				newName = "";
			}
			SetName(newName);
		}
	}

	void Entity::TransformGUI()
	{
		//TRANSFORM
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position");
			float x = m_Transform.position.x;
			float y = m_Transform.position.y;
			float z = m_Transform.position.z;

			constexpr float STEP_FINE = 0.5f;
			constexpr float STEP_FAST = 10.0f; //Hold down Ctrl to scroll faster.

			ImGui::InputFloat("x", &x, STEP_FINE, STEP_FAST);
			//ImGui::SameLine();
			ImGui::InputFloat("y", &y, STEP_FINE, STEP_FAST);
			//ImGui::SameLine();
			ImGui::InputFloat("z", &z, STEP_FINE, STEP_FAST);

			//NOTE: If the position is not changing, it's probably because the physics engine (or something else) is overwriting it)
			SetPosition(glm::vec3(x, y, z));
		}
	}

	btTransform Entity::getBTTransform()
	{
		btTransform btTrans;
		btTrans.setIdentity();
		btTrans.setOrigin(btVector3{ m_Transform.position.x, m_Transform.position.y, m_Transform.position.z });
		return btTrans;
	}

	void Entity::setFromBTTransform(const btTransform& btTrans)
	{
		m_Transform.position = btTransformToVec3(btTrans);
	}

	void Entity::ShowInspectorGUI()
	{
		ImGui::Begin("Inspector");

		NameGUI();
		TransformGUI();

		ImGui::SetWindowSize(ImVec2(300, 800), ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(300, 0), ImGuiCond_Once);
		ImGui::End();
	}
} // namespace lei3d