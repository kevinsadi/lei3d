#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace lei3d
{
	class UiMesh;

	class GuiComponent
	{
	public:
		enum Anchor
		{
			TOP_LEFT,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			CENTER_TOP,
			CENTER_RIGHT,
			CENTER_BOTTOM,
			CENTER_LEFT,
			CENTER,
			ANCHOR_COUNT
		};

		GuiComponent(Anchor anchor, glm::vec2 pos = { 0.25, 0.25 }, glm::vec2 size = { 0.5, 0.5 });
		virtual ~GuiComponent();

	protected:
		friend class GuiManager;

		static unsigned s_nextId;
		unsigned m_id;
		unsigned m_anchor;

		virtual void Render(const glm::vec2& screenSize) = 0;
		virtual void Update() = 0;

		static const glm::vec3 s_anchorPositions[ANCHOR_COUNT];

		glm::vec2 m_position; // normalized 0-1
		glm::vec2 m_size;	  // normalized 0-1
		UiMesh* m_mesh = nullptr;
	};
}
