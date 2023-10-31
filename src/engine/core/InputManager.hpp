#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace lei3d
{

	class InputManager
	{
	public:
		static InputManager& GetInstance();
		~InputManager();

		static void initialize(GLFWwindow* window);
		static void update();

		bool isKeyDown(unsigned int key_id);
		bool isKeyPressed(unsigned int key_id);
		bool isButtonDown(unsigned int butt_id);

		glm::vec2 getMouseOffset() { return mouse_offset; }
		glm::vec2 getMousePosition() { return mouse_coords; }

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
	};

} // namespace lei3d
