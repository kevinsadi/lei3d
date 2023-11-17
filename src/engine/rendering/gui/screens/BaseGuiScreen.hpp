#pragma once
#include "GuiScreen.hpp"
#include "core/Application.hpp"

namespace lei3d
{
	class BaseGuiScreen : public GuiScreen
	{
	private:
		int m_flowersFoundId = -1;
		int m_timerTextboxId = -1;
		float m_timer = 0;
		float m_flowersFound = 0;

	protected:
		void Init() override;

	public:
		BaseGuiScreen();
		~BaseGuiScreen() override;

		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos) override;
		void AddFlower();
	};
} // namespace lei3d
