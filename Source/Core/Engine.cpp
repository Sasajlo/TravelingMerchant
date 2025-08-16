#include "Engine.hpp"

#include <Core/Input.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

bool Engine::Initialize(const std::string& title, int width, int height, bool fullscreen)
{
	// Initialize window
	if (!Instance()._window.Initialize(title, width, height, fullscreen)) {
		std::cerr << "Failed to initialize window!" << std::endl;
		return false;
	};

	std::cout << "Engine initialized!" << std::endl;
	return true;
}

void Engine::Run()
{
	int fps = 0;
	std::cout << "Engine is running!" << std::endl;
	while (!Instance()._window.ShouldClose())
	{
		// Get the active scene from the SceneManager
		auto currentScene = SceneManager::GetActiveScene();

		// Initialize asleep objects in the current scene
		currentScene->Awake();
		currentScene->Start();

		// Handle delta time calculation
		Instance()._time.Update();

		int newFps = Instance().GetFPS();
		if (fps != newFps)
		{
			fps = newFps;
			Instance()._window.UpdateFPS(fps);
		}

		// Poll for events
		Instance()._window.PollEvents();

		if (Input::IsKeyPressed(GLFW_KEY_F11)) {
			Instance()._window.ToggleFullscreen();
		}

		// Check if window is minimized
		if (GetWindowSize().width != 0 && GetWindowSize().height != 0)
		{
			// Update the active scene
			currentScene->Update(Instance()._time.GetDeltaTime());

			// Clear the window from previous frame
			Instance()._window.Clear(Camera::GetMain()->GetBackgroundColor());

			// Render the active scene
			currentScene->Render();
		}

		// Swap buffers
		Instance()._window.SwapBuffers();

		// Reset input states
		Input::Reset();
	}
}

void Engine::Stop()
{
	
}

void Engine::Shutdown()
{
	std::cout << "Engine shutting down!" << std::endl;
	SceneManager::Destroy();
	Instance()._window.Destroy();
	Texture::ClearCache();
}

void Engine::RenderFPS(int fps)
{
	Instance()._window.UpdateFPS(fps);
}
