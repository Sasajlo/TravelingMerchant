#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Utils/Color.hpp>

using namespace TM::Utils;

namespace TM
{
	namespace Core 
	{
		class Window
		{
		public:
			Window() = default;
			~Window() = default;

			static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

			bool Initialize(const std::string& title, int width=600, int height=400);
			void Close();
			void Destroy();


			void Clear(Color color);
			void SwapBuffers();
			void PollEvents();

			bool ShouldClose() const;

			void UpdateFPS(int FPS);

			int GetWidth() const;
			int GetHeight() const;

		private:
			GLFWwindow* _window;
			std::string _title;
		};
	}
}