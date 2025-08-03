#include <iostream>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Core/GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Utils/Color.hpp>
#include <Game/Player.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Utils;
using namespace TM::Game;

int main()
{
	Engine engine;
	if (!engine.Initialize("Traveling Merchant", 800, 600)) {
		std::cerr << "Failed to initialize engine!" << std::endl;
		return -1;
	}

	// Create main scene
	SceneManager::AddScene("Main Scene");

	// Create camera game object to main scene
	GameObject* cameraObject = GameObject::Create("Main Camera");
	Camera* camera = cameraObject->AddComponent<Camera>();
	camera->SetBackgroundColor(Color::GRAY);

	// Create player game object to main scene
	GameObject* playerObject = GameObject::Create("Player");
	Player* player = playerObject->AddComponent<Player>();
	player->CustomMethod(); // Call custom method to demonstrate functionality

	engine.Run();
	engine.Shutdown();

	return 0;
}