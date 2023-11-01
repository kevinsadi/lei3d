#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

namespace lei3d
{
	class GuiComponent;

	class GuiScreen
    {
    private:
		std::unordered_map<unsigned, GuiComponent*> m_components;

	public:
		GuiScreen();
		virtual ~GuiScreen();

		virtual void Init();

		void AddComponent(GuiComponent* guiComponent);
		bool RemoveComponent(unsigned id);

		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos);
		void Render(const glm::vec2& screenSize);
    };
} 
