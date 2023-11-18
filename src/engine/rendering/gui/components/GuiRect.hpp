#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "GuiComponent.hpp"

namespace lei3d 
{
    class GuiRect : public GuiComponent
    {
    protected:
        glm::vec4 m_color;
		int m_textureId;

    public:
        GuiRect(
            Anchor anchor = Anchor::CENTER, 
            const std::pair<Space, glm::vec2>& pos = { Space::NORMALIZED, { 0.25, 0.25 } }, 
            const std::pair<Space, glm::vec2>& size = { Space::NORMALIZED, { 0.5, 0.5 } },
            const glm::vec4& color = { 1, 0, 0, 0.5 },
            int m_textureId = -1,
			std::function<void()> onClick = nullptr,
			std::function<void()> onHover = nullptr,
			std::function<void()> onStopHover = nullptr
        );
		~GuiRect() override;

        void SetColor(const glm::vec4& color);

        void Render(const glm::vec2& screenSize) override;
        void Update() override;

        bool IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const override;
    };
}
