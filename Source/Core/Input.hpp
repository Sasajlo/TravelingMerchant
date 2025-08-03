#pragma once

#include <iostream>
#include <unordered_map>
#include <GLFW/glfw3.h>

namespace TM
{
	namespace Core
	{
		class Input
		{
		public:
			~Input() = default;

			static bool IsKeyPressed(int key);
			static bool IsKeyReleased(int key);
			static bool IsKeyHeld(int key);

			static void Initialize(GLFWwindow* window);
			static void Destroy();
			
			// Update key states (call at end of frame)
			static void Update();

		private:
			Input() = default;

			static Input& Instance()
			{
				static Input instance;
				return instance;
			}

			GLFWwindow* _window = nullptr;
			std::unordered_map<int, bool> _keyStates; // Track current key states
			std::unordered_map<int, bool> _previousKeyStates; // Track previous frame key states
		};
	}
}