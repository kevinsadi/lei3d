#include "core/InputManager.hpp"

namespace lei3d
{

	InputManager::InputManager()
	{
	}

	InputManager& InputManager::GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::initialize(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, InputManager::keyCallback);
		glfwSetMouseButtonCallback(window, InputManager::mouseButtonCallback);
		glfwSetCursorPosCallback(window, InputManager::mouseCallback);
	}

	void InputManager::update()
	{
		InputManager::GetInstance().setMouseOffset();
	}

	bool InputManager::isKeyDown(unsigned int key_id)
	{
		bool result = false;
		auto it = key_down_map.find(key_id);
		if (it != key_down_map.end()) {
			result = key_down_map[key_id];
		}
		return result;
	}

	bool InputManager::isKeyPressed(unsigned int key_id)
	{
		bool result = false;
		auto it = key_pressed_map.find(key_id);
		if (it != key_pressed_map.end()) {
			result = key_pressed_map[key_id];
			key_pressed_map[key_id] = false;
		}
		return result;
	}

	bool InputManager::isButtonDown(unsigned int butt_id)
	{
		bool result = false;
		auto it = mb_map.find(butt_id);
		if (it != mb_map.end()) {
			result = mb_map[butt_id];
		}
		return result;
	}

	void InputManager::setKeyDown(unsigned int key_id, bool is_down)
	{
		key_down_map[key_id] = is_down;
	}

	void InputManager::setKeyPressed(unsigned int key_id)
	{
		auto it = key_pressed_map.find(key_id);
		if (it != key_pressed_map.end()) {
			key_pressed_map[key_id] = !key_pressed_map[key_id];
		} else {
			key_pressed_map[key_id] = true;
		}
	}

	void InputManager::setButtonDown(unsigned int butt_id, bool is_down)
	{
		mb_map[butt_id] = is_down;
	}

	void InputManager::setMouseCoords(glm::vec2 coords)
	{
		if (first_mouse_input) {
			mouse_coords = coords;
			first_mouse_input = false;
		}

		mouse_coords = coords;
	}

	void InputManager::setMouseOffset()
	{
		mouse_offset = mouse_coords - prev_mouse_coords;
		prev_mouse_coords = mouse_coords;

		if (!invert_y) {
			mouse_offset.y = -mouse_offset.y;
		}
	}

	void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputManager& im = InputManager::GetInstance();
		im.setKeyDown(key, action != GLFW_RELEASE);
		if (action == GLFW_PRESS) {
			im.setKeyPressed(key);
		}
	}

	void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		InputManager& im = InputManager::GetInstance();
		im.setButtonDown(button, action != GLFW_RELEASE);
	}

	void InputManager::mouseCallback(GLFWwindow* window, double x_pos_in, double y_pos_in)
	{
		InputManager& im = InputManager::GetInstance();
		im.setMouseCoords(glm::vec2(x_pos_in, y_pos_in));
	}

} // namespace lei3d