#include "GuiTextureRect.hpp"

#include "core/InputManager.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/UiMesh.hpp"

namespace lei3d
{
	GuiTextureRect::GuiTextureRect(
		int textureID,
		Anchor anchor,
		const std::pair<Space, glm::vec2>& pos,
		const std::pair<Space, glm::vec2>& size,
		std::function<void()> onClick
	)
		: GuiComponent(anchor, pos, size, onClick)
		, m_textureID(textureID)
	{
		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 0.0f }, UiMesh::Vec2{0.f, 0.f});
		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 1.0f }, UiMesh::Vec2{ 0.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 1.0f }, UiMesh::Vec2{ 1.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 0.0f }, UiMesh::Vec2{ 1.0f, 1.0f });

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(0);
		indices.emplace_back(2);
		indices.emplace_back(3);

		m_mesh = new UiMesh(vertices, indices, textureID);
	}

	GuiTextureRect::~GuiTextureRect()
	{
		delete m_mesh;
	}

	void GuiTextureRect::Render(const glm::vec2& screenSize)
	{
		GuiManager::Instance().m_guiShader.bind();

		GuiManager::Instance().m_guiShader.setUniformMat4("transform",
			glm::translate(glm::identity<glm::mat4>(), PosNormalized(screenSize)) * glm::scale(glm::identity<glm::mat4>(), glm::vec3(SizeNormalized(screenSize), 1)));
		
		GuiManager::Instance().m_guiShader.setVec4("color", {1, 1, 1, 1});

		m_mesh->Draw(&GuiManager::Instance().m_guiShader);
	}

	void GuiTextureRect::Update()
	{
		if (m_mouseOver && m_onClick && InputManager::GetInstance().isButtonDown(GLFW_MOUSE_BUTTON_LEFT, InputManager::InputTarget::GUI))
		{
			m_onClick();
		}
	}

	bool GuiTextureRect::IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const
	{
		const glm::vec3 pos = PosPixels(screenSize);
		const glm::vec2 size = SizePixels(screenSize);

		bool mouseOver = mousePosition.x >= pos.x && mousePosition.x <= pos.x + size.x
			&& mousePosition.y >= pos.y && mousePosition.y <= pos.y + size.y;

		return mouseOver;
	}
} 