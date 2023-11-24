#pragma once
#include "GuiScreen.hpp"
#include <time.h>

namespace lei3d 
{
    class MadeInLei3dScreen : public GuiScreen
    {
    private:
		static int GetLei3dTexture();
        clock_t start;

    protected:
        void Init() override;

    public:
        MadeInLei3dScreen();
        ~MadeInLei3dScreen() override;

        void HandleInput() override;
        void Update(const glm::vec2& screenSize, const glm::vec2& mousePos) override;
    };
} // namespace lei3d