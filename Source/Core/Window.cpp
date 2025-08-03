#include "Window.hpp"

#include <format>
#include <Core/Input.hpp>

using namespace TM::Core;

bool Window::Initialize(const std::string& title, int width, int height)
{
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return false;
	}


	// Create a GLFW window
	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!_window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}

	_title = title; // Save the title locally

	// Set the window's context	
	glfwMakeContextCurrent(_window);

	// Disabvle V-Sync
	glfwSwapInterval(0);

	// Initialize Input system
	Input::Initialize(_window);

	int _width, _height;
	std::cout << "Window initialized: " << glfwGetWindowTitle(_window) << " (" << GetWidth() << "x" << GetHeight() << ")" << std::endl;
	return true;
}

void Window::Close()
{
	glfwSetWindowShouldClose(_window, true);
}

void Window::Destroy()
{
	glfwTerminate();
	std::cout << "Window terminated successfully." << std::endl;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(_window);
}

void Window::UpdateFPS(int FPS)
{
	glfwSetWindowTitle(_window, std::format("{} - FPS: {}", _title, FPS).c_str());
}

int Window::GetWidth() const
{
	if (!_window) {
		std::cerr << "Window is not initialized." << std::endl;
		return 0;
	}

	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	return width;
}

int Window::GetHeight() const
{
	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	return height;
}

void Window::Clear(Color color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}
