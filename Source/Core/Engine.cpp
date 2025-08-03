#include "Engine.hpp"

#include <Core/Input.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/Camera.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

Engine::Engine()
{

}

Engine::~Engine()
{
}

bool Engine::Initialize(const std::string& title, int width, int height)
{
	// Initialize window
	if (!_window.Initialize(title, width, height)) {
		std::cerr << "Failed to initialize window!" << std::endl;
		return false;
	};

	std::cout << "Engine initialized!" << std::endl;
	return true;
}

void Engine::Run()
{
	std::cout << "Engine is running!" << std::endl;
	while (!_window.ShouldClose())
	{
		// Get the active scene from the SceneManager
		auto currentScene = SceneManager::GetActiveScene();

		// Initialize asleep objects in the current scene
		currentScene->Awake();
		currentScene->Start();

		// Handle delta time calculation
		_time.Update();

		// Update the window title with the current FPS
		_window.UpdateFPS(_time.GetFPS());

		// Update input states
		Input::Update();

		// Update the active scene
		currentScene->Update(_time.GetDeltaTime());

		// Clear the window from previous frame
		_window.Clear(Camera::GetMain()->GetBackgroundColor());

		// Render the active scene
		currentScene->Render();

		// Swap buffers
		_window.SwapBuffers();

		// Poll for events
		_window.PollEvents();
	}
}

void Engine::Stop()
{
	
}

void Engine::Shutdown()
{
	std::cout << "Engine shutting down!" << std::endl;
	SceneManager::Destroy();
	_window.Destroy();
}
