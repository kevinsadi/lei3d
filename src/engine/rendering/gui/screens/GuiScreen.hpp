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
		std::unordered_map<unsigned, GuiComponent*> m_components;
		bool m_initialized = false;

	protected:
		virtual void Init();

	public:
		GuiScreen();
		virtual ~GuiScreen();

		void AddComponent(GuiComponent* guiComponent);
		bool RemoveComponent(unsigned id);

		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos);
		void Render(const glm::vec2& screenSize);
    };
} 
