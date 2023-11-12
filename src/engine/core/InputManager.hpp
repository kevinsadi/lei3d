#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace lei3d
{

	class InputManager
	{
	public:
		enum class InputTarget
		{
			GAME,
			IMGUI,
			GUI,
			NONE
		};

		static InputManager& GetInstance();
		~InputManager();

		static void initialize(GLFWwindow* window);

		void update(GLFWwindow* window);

		bool isKeyDown(unsigned int key_id, InputTarget for_target = InputTarget::GAME);
		bool isKeyPressed(unsigned int key_id, InputTarget for_target = InputTarget::GAME);
		bool isButtonDown(unsigned int butt_id, InputTarget for_target = InputTarget::GAME);

		glm::vec2 getMouseOffset(InputTarget for_target = InputTarget::GAME);
		glm::vec2 getMousePosition() { return mouse_coords; }

		void giveInputFocus(InputTarget target) { has_focus = target; }
		InputTarget inputFocus() const { return has_focus; }

	private:
		InputManager();

		void setKeyDown(unsigned int key_id, bool is_down);
		void setKeyPressed(unsigned int key_id);

		void setButtonDown(unsigned int butt_id, bool is_down);

		void setMouseCoords(glm::vec2 coords);
		void setMouseOffset();

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in);

		std::unordered_map<unsigned int, bool> key_down_map;
		std::unordered_map<unsigned int, bool> key_pressed_map;

		std::unordered_map<unsigned int, bool> mb_map;

		glm::vec2 mouse_coords;
		glm::vec2 prev_mouse_coords;
		glm::vec2 mouse_offset;
		bool invert_y = false;
		bool first_mouse_input = true;
		InputTarget has_focus = InputTarget::GAME;
	};

} // namespace lei3d
