#include "Input.hpp"

using namespace TM::Core;

void Input::Initialize(GLFWwindow* window)
{
	Instance()._window = window;
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

void Input::Destroy()
{
	auto& instance = Instance();
	instance._window = nullptr;
	instance._keyStates.clear();
	instance._previousKeyStates.clear();
}

// Call this at the end of each frame to update previous key states
void Input::Update()
{
	auto& instance = Instance();
	instance._previousKeyStates = instance._keyStates;
}
