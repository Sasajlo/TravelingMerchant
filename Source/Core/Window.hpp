#pragma once

#include <iostream>
#include <Graphics/Core.hpp>

#include <Utils/Color.hpp>

using namespace TM::Utils;

namespace TM
{
	namespace Core 
	{
		class Window
		{
		private:
			GLFWwindow* _window;
			std::string _title;

			bool _isFullscreen = false;
			int _windowedX = 100;
			int _windowedY = 100;
			int _windowedWidth = 800;
			int _windowedHeight = 600;

		public:
			Window() = default;
			~Window() = default;

			static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

			bool Initialize(const std::string& title, int width=600, int height=400, bool fullscreen=false);
			void Close();
			void Destroy();

			void Clear(Color color);
			void SwapBuffers();
			void PollEvents();

			bool ShouldClose() const;

			void UpdateFPS(int FPS);

			int GetWidth() const;
			int GetHeight() const;

			void ToggleFullscreen();
			bool IsFullscreen() const { return _isFullscreen; }
		};
	}
}