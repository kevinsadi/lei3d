#include "GuiComponent.hpp"

#include "rendering/gui/GuiManager.hpp"

namespace lei3d
{
	unsigned GuiComponent::s_nextId = 0;

	const glm::vec3 GuiComponent::s_anchorPositions[ANCHOR_COUNT] = 
	{
		{0.0f, 1.0f, 0},	// TOP_LEFT
		{1.0f, 1.0f, 0},	// TOP_RIGHT
		{0.0f, 0.0f, 0},	// BOTTOM_LEFT
		{1.0f, 0.0f, 0},	// BOTTOM_RIGHT
		{0.5f, 1.0f, 0},	// CENTER_TOP
		{1.0f, 0.5f, 0},	// CENTER_RIGHT
		{0.5f, 0.0f, 0},	// CENTER_BOTTOM
		{0.0f, 0.5f, 0},	// CENTER_LEFT
		{0.5f, 0.5f, 0}	// CENTER
	};

	GuiComponent::GuiComponent(Anchor anchor, glm::vec2 pos, glm::vec2 size)
		: m_anchor(anchor)
		, m_position(pos)
		, m_size(size)
	{
		m_id = s_nextId++;
		GuiManager::Instance().AddGuiComponent(this);
	}

	GuiComponent::~GuiComponent()
	{
		GuiManager::Instance().RemoveGuiComponent(m_id);
	}
}
