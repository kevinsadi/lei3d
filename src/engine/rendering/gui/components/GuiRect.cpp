#include "GuiRect.hpp"

#include "core/InputManager.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/UiMesh.hpp"
#include "logging/Log.hpp"

namespace lei3d 
{
	GuiRect::GuiRect(
		Anchor anchor, 
		const std::pair<Space, glm::vec2>& pos, 
		const std::pair<Space, glm::vec2>& size,
		std::function<void()> onClick,
		glm::vec4 color
	)
		: GuiComponent(anchor, pos, size, onClick)
		, m_color(color)
	{
		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 0.0f });
		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 0.0f });

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);

		indices.emplace_back(0);
		indices.emplace_back(3);
		indices.emplace_back(2);

		m_mesh = new UiMesh(vertices, indices);
	}

	GuiRect::~GuiRect()
	{
		delete m_mesh;
	}

	void GuiRect::SetColor(const glm::vec4& color)
	{
		m_color = color;
	}

	void GuiRect::Render(const glm::vec2& screenSize)
	{
		GuiManager::Instance().m_guiShader.bind();

		GuiManager::Instance().m_guiShader.setUniformMat4("translation",
			glm::translate(glm::identity<glm::mat4>(), PosNormalized(screenSize)));

		GuiManager::Instance().m_guiShader.setUniformMat4("scale",
			glm::scale(glm::identity<glm::mat4>(), glm::vec3(SizeNormalized(screenSize), 1)));
		
		GuiManager::Instance().m_guiShader.setVec4("color", m_color);

		m_mesh->Draw(&GuiManager::Instance().m_guiShader);
	}

	void GuiRect::Update()
	{
		if (m_mouseOver && m_onClick && InputManager::GetInstance().isButtonDown(GLFW_MOUSE_BUTTON_LEFT, InputManager::InputTarget::GUI))
		{
			m_onClick();
		}
	}

	bool GuiRect::IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const
	{
		const glm::vec3 pos = PosPixels(screenSize);
		const glm::vec2 size = SizePixels(screenSize);

		bool mouseOver = mousePosition.x >= pos.x && mousePosition.x <= (pos.x + size.x)
						&& mousePosition.y >= pos.y && mousePosition.y <= (pos.y + size.y);

		return mouseOver;
	}
}
