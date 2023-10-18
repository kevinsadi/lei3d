#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "GuiComponent.hpp"

namespace lei3d 
{
    class GuiRect : public GuiComponent
    {
    private:
        glm::vec4 m_color;

    public:
        GuiRect(
            Anchor anchor = CENTER, 
            const std::pair<Space, glm::vec2>& pos = { NORMALIZED, { 0.25, 0.25 } }, 
            const std::pair<Space, glm::vec2>& size = { NORMALIZED, { 0.5, 0.5 } }, 
            glm::vec4 color = { 1, 0, 0, 0.5 }
        );
		~GuiRect() override;

        void SetColor(const glm::vec4& color);

        void Render(const glm::vec2& screenSize) override;
        void Update() override;
    };
}
