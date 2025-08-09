#include "Engine.hpp"

#include <Core/Input.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Texture.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

bool Engine::Initialize(const std::string& title, int width, int height)
{
	// Initialize window
	if (!Instance()._window.Initialize(title, width, height)) {
		std::cerr << "Failed to initialize window!" << std::endl;
		return false;
	};

	std::cout << "Engine initialized!" << std::endl;
	return true;
}

void Engine::Run()
{
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

		// Poll for events
		Instance()._window.PollEvents();

		// Update the active scene
		currentScene->Update(Instance()._time.GetDeltaTime());

		// Clear the window from previous frame
		Instance()._window.Clear(Camera::GetMain()->GetBackgroundColor());

		// Render the active scene
		currentScene->Render();

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
