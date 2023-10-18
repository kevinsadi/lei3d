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
		GuiRect(Anchor anchor, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		~GuiRect() override;

        void Render(const glm::vec2& screenSize) override;
        void Update() override;
    };
}
