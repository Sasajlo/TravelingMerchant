#include <Core/Window.hpp>

#include <format>
#include <Core/Input.hpp>
#include <iostream>
#include <vector>
#include <Utils/File.hpp>
#include <Graphics/Shader.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

// Function to print available GPUs and their capabilities
void PrintGPUInfo() {
    std::cout << "=== GPU Information ===" << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "=========================" << std::endl;
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// Update the viewport
	glViewport(0, 0, width, height);

	std::cout << "Framebuffer resized: " << width << "x" << height << std::endl;
}

bool Window::Initialize(const std::string& title, int width, int height)
{
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n";
		return false;
	}

	// Set hints for GPU selection
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	
	// For NVIDIA GPUs, you can try to force the discrete GPU
	// This works on some systems but not all
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	_title = title; // Save the title locally

	// Create a GLFW window
	_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!_window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return false;
	}


	// Set the window's context	
	glfwMakeContextCurrent(_window);

	
	// Load GLAD to get OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}
	
	// Print GPU information
	PrintGPUInfo();

	// Enable depth testing for 3D rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Optional: Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Viewport
	glViewport(0, 0, width, height);

	// Set the framebuffer size callback
	glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}
