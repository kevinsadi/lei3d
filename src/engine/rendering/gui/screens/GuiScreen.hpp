#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

namespace lei3d
{
	class GuiComponent;

	class GuiScreen
    {
		friend class GuiManager;

    private:
		bool m_initialized = false;

	protected:
		bool m_shouldHideHUD = false;
		virtual void Init();
		std::unordered_map<unsigned, GuiComponent*> m_components;

	public:
		GuiScreen();
		virtual ~GuiScreen();

		void AddComponent(GuiComponent* guiComponent);
		bool RemoveComponent(unsigned id);

		virtual void HandleInput();
		virtual void Update(const glm::vec2& screenSize, const glm::vec2& mousePos);
		void Render(const glm::vec2& screenSize);
    };
} 
