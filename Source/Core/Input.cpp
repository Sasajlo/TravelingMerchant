#include "Input.hpp"

using namespace TM::Core;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::SetScrollDelta(static_cast<float>(yoffset));
}

void Input::Initialize(GLFWwindow* window)
{
	Instance()._window = window;
	glfwSetScrollCallback(window, scroll_callback);
}

bool Input::IsKeyPressed(int key)
{
	auto& instance = Instance();
	
	// Check if key is currently pressed and wasn't pressed in the previous frame
	bool currentState = glfwGetKey(instance._window, key) == GLFW_PRESS;
	bool previousState = instance._previousKeyStates[key];
	
	// Update current state
	instance._keyStates[key] = currentState;
	
	// Return true if key is pressed this frame but wasn't pressed last frame
	return currentState && !previousState;
}

bool Input::IsKeyReleased(int key)
{
	auto& instance = Instance();
	
	// Check if key is currently released and was pressed in the previous frame
	bool currentState = glfwGetKey(instance._window, key) == GLFW_PRESS;
	bool previousState = instance._previousKeyStates[key];
	
	// Update current state
	instance._keyStates[key] = currentState;
	
	// Return true if key is released this frame but was pressed last frame
	return !currentState && previousState;
}

bool Input::IsKeyHeld(int key)
{
	auto& instance = Instance();
	
	// Check if key is currently pressed
	bool currentState = glfwGetKey(instance._window, key) == GLFW_PRESS;
	
	// Update current state
	instance._keyStates[key] = currentState;
	
	// Return true if key is currently held down
	return currentState;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto& instance = Instance();
	bool currentState = glfwGetMouseButton(instance._window, button) == GLFW_PRESS;
	bool previousState = instance._previousMouseButtonStates[button];
	instance._mouseButtonStates[button] = currentState;
	return currentState && !previousState;
}

bool Input::IsMouseButtonReleased(int button)
{
	auto& instance = Instance();
	bool currentState = glfwGetMouseButton(instance._window, button) == GLFW_PRESS;
	bool previousState = instance._previousMouseButtonStates[button];
	instance._mouseButtonStates[button] = currentState;
	return !currentState && previousState;
}

bool Input::IsMouseButtonHeld(int button)
{
	auto& instance = Instance();
	bool currentState = glfwGetMouseButton(instance._window, button) == GLFW_PRESS;
	instance._mouseButtonStates[button] = currentState;
	return currentState;
}

// Add these new methods
float Input::GetScrollDelta()
{
	return Instance()._scrollDelta;
}

void Input::SetScrollDelta(float delta)
{
	Instance()._scrollDelta = delta;
}

Vector3 Input::GetMousePosition()
{
	auto& instance = Instance();
	return Vector3(static_cast<float>(instance._mouseX), static_cast<float>(instance._mouseY), 0.0f);
}

void Input::UpdateMousePosition()
{
	glfwGetCursorPos(_window, &_mouseX, &_mouseY);
}

void Input::Destroy()
{
	auto& instance = Instance();
	instance._window = nullptr;
	instance._keyStates.clear();
	instance._previousKeyStates.clear();
	instance._mouseButtonStates.clear();
	instance._previousMouseButtonStates.clear();
}

// Call this at the end of each frame to update previous key states
void Input::Reset()
{
	auto& instance = Instance();
	instance._previousKeyStates = instance._keyStates;
	instance._previousMouseButtonStates = instance._mouseButtonStates;
	instance._scrollDelta = 0.0f;  // Reset scroll delta each frame
	instance.UpdateMousePosition();
}
