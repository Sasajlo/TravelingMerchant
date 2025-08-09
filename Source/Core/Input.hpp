#pragma once

#include <iostream>
#include <unordered_map>
#include <Graphics/Core.hpp>
#include <Utils/Vector3.hpp>

using namespace TM::Utils;

namespace TM
{
	namespace Core
	{
		class Input
		{
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
			float _scrollDelta = 0.0f; // Add this new member

			// Mouse position
			double _mouseX = 0.0;
			double _mouseY = 0.0;

			std::unordered_map<int, bool> _mouseButtonStates;
			std::unordered_map<int, bool> _previousMouseButtonStates;


		public:
			~Input() = default;

			static bool IsKeyPressed(int key);
			static bool IsKeyReleased(int key);
			static bool IsKeyHeld(int key);

			static bool IsMouseButtonPressed(int button);
			static bool IsMouseButtonReleased(int button);
			static bool IsMouseButtonHeld(int button);

			static float GetScrollDelta();
			static void SetScrollDelta(float delta);

			static Vector3 GetMousePosition();
			void UpdateMousePosition();

			static void Initialize(GLFWwindow* window);
			static void Destroy();
			
			// Update key states (call at end of frame)
			static void Reset();
		};
	}
}