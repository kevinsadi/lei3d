#include "PauseMenuScreen.hpp"

#include "SplashScreen.hpp"
#include "core/Application.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"
#include "Logging/Log.hpp"
#include "rendering/gui/GuiManager.hpp"

namespace lei3d
{
	PauseMenuScreen::PauseMenuScreen()
	{
		
	}

	PauseMenuScreen::~PauseMenuScreen()
	{
	}

	void PauseMenuScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT, 
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } }, 
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } }, 
			{ 0, 0, 0, 0.5 }
		));

		GuiTextBox* resumeButton = new GuiTextBox(
			"Resume",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, -0.1 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				GuiManager::Instance().CloseActiveScreen();
			});

		resumeButton->SetOnHover([this, resumeButton]() {
			((GuiTextBox*)(this->m_components[resumeButton->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		resumeButton->SetOnStopHover([this, resumeButton]() {
			((GuiTextBox*)(this->m_components[resumeButton->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		resumeButton->m_alignCenter = true;

		GuiTextBox* restartButton = new GuiTextBox(
			"Restart",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				SceneManager::ActiveScene().Reset();
				GuiManager::Instance().CloseActiveScreen();
			});

		restartButton->SetOnHover([this, restartButton]() {
			((GuiTextBox*)(this->m_components[restartButton->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		restartButton->SetOnStopHover([this, restartButton]() {
			((GuiTextBox*)(this->m_components[restartButton->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		restartButton->m_alignCenter = true;

		GuiTextBox* exitButton = new GuiTextBox(
			"Exit",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0.1 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				glfwSetWindowShouldClose(Application::Window(), true);
			});

		exitButton->SetOnHover([this, exitButton]() {
			((GuiTextBox*)(this->m_components[exitButton->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		exitButton->SetOnStopHover([this, exitButton]() {
			((GuiTextBox*)(this->m_components[exitButton->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		exitButton->m_alignCenter = true;

		AddComponent(resumeButton);
		AddComponent(restartButton);
		AddComponent(exitButton);	
	}
} 