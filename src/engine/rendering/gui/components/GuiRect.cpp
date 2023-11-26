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
		const glm::vec4& color,
		int m_textureId,
		std::function<void()> onClick,
		std::function<void()> onHover,
		std::function<void()> onStopHover
	)
		: GuiComponent(anchor, pos, size, onClick, onHover, onStopHover)
		, m_color(color)
		, m_textureId(m_textureId)
	{
		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 0.0f }, UiMesh::Vec2{ 0.0f, 0.0f });
		vertices.emplace_back(UiMesh::Vec2{ 0.0f, 1.0f }, UiMesh::Vec2{ 0.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 1.0f }, UiMesh::Vec2{ 1.0f, 1.0f });
		vertices.emplace_back(UiMesh::Vec2{ 1.0f, 0.0f }, UiMesh::Vec2{ 1.0f, 0.0f });

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);

		indices.emplace_back(0);
		indices.emplace_back(3);
		indices.emplace_back(2);

		m_pMesh = new UiMesh(vertices, indices, m_textureId);
	}

	GuiRect::~GuiRect()
	{
		delete m_pMesh;
	}

	void GuiRect::SetColor(const glm::vec4& color)
	{
		m_color = color;
	}

	void GuiRect::Render(const glm::vec2& screenSize)
	{
		m_pShader->bind();

		m_pShader->setUniformMat4("translation", translate(glm::identity<glm::mat4>(), PosNormalized(screenSize)));

		m_pShader->setUniformMat4("scale", scale(glm::identity<glm::mat4>(), glm::vec3(SizeNormalized(screenSize), 1)));
		
		m_pShader->setVec4("color", m_color);

		m_pShader->setInt("useTex", m_textureId == -1 ? 0 : 1);
		m_pShader->setInt("ourTexture", 0);

		m_pMesh->Draw(m_pShader);
	}

	void GuiRect::Update()
	{
		if (m_mouseOver)
		{
			if (m_onHover)
			{
				m_onHover();
			}

			if (m_onClick && InputManager::GetInstance().isButtonDown(GLFW_MOUSE_BUTTON_LEFT, InputManager::InputTarget::GUI))
			{
				m_onClick();
			}
		}
		else if (m_mouseOverLast && m_onStopHover)
		{
			m_onStopHover();
		}
	}

	bool GuiRect::IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const
	{
		const glm::vec3 pos = PosPixels(screenSize);
		const glm::vec2 size = SizePixels(screenSize);

		bool mouseOver = mousePosition.x >= pos.x && mousePosition.x <= (pos.x + size.x)
						&& mousePosition.y >= (pos.y - (size.y / 2)) && mousePosition.y <= (pos.y + (size.y / 2));

		return mouseOver;
	}
}
