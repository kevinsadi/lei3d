#pragma once
#include <functional>
#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace lei3d
{
	class UiMesh;

	class GuiComponent
	{
	public:
		enum class Anchor
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

		enum class Space
		{
			PIXELS,
			NORMALIZED
		};

		GuiComponent(
			Anchor anchor = Anchor::CENTER, 
			const std::pair<Space, glm::vec2>& pos = { Space::NORMALIZED, { 0.25, 0.25 } }, 
			const std::pair<Space, glm::vec2>& size = { Space::NORMALIZED, { 0.5, 0.5 } },
			std::function<void()> onClick = nullptr
		);
		virtual ~GuiComponent();

		void SetPositionNormalized(const glm::vec2& pos);
		void SetPositionPixels(const glm::vec2& pos);

		void SetSizeNormalized(const glm::vec2& size);
		void SetSizePixels(const glm::vec2& size);

		bool GetMouseOver();

		unsigned GetId() const;

	protected:
		friend class GuiScreen;

		static unsigned s_nextId;
		unsigned m_id;
		unsigned m_anchor;

		static const glm::vec3 s_anchorPositions[unsigned(Anchor::ANCHOR_COUNT)];

		std::pair<Space, glm::vec2> m_position;
		std::pair<Space, glm::vec2> m_size;	
		UiMesh* m_mesh = nullptr;
		std::function<void()> m_onClick;
		bool m_mouseOver = false;

		void BeginRender();
		void EndRender();

		virtual void Render(const glm::vec2& screenSize) = 0;
		virtual void Update() = 0;
		virtual bool IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const = 0;

		glm::vec3 PosNormalized(const glm::vec2& screenSize) const;
		glm::vec2 SizeNormalized(const glm::vec2& screenSize) const;

		glm::vec3 PosPixels(const glm::vec2& screenSize) const;
		glm::vec2 SizePixels(const glm::vec2& screenSize) const;
	};
}
