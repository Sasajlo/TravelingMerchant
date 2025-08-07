#include <iostream>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Core/GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Color.hpp>
#include <Game/Player.hpp>
#include <Game/Billboard.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Utils;
using namespace TM::Game;

int main()
{
	if (!Engine::Initialize("Traveling Merchant", 800, 600)) {
		std::cerr << "Failed to initialize engine!" << std::endl;
		return -1;
	}

	// Create main scene
	SceneManager::AddScene("Main Scene");

	// Create camera object
	GameObject* cameraObject = GameObject::Create("Main Camera");
	Camera* camera = cameraObject->AddComponent<Camera>();
	camera->SetBackgroundColor(Color::GRAY);

	// Create player object
	GameObject* playerObject = GameObject::Create("Player");
	playerObject->AddComponent<Player>();
	Sprite* playerSprite = playerObject->AddComponent<Sprite>();
	playerSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
	playerSprite->SetTexture("Assets/Textures/player.png");
	playerObject->AddComponent<SpriteRenderer>();
	playerObject->AddComponent<Billboard>();

	camera->LookAt(playerObject); // Set camera to follow player
	camera->Follow(playerObject, {}); // Set camera to follow player

	// Create ground object
	GameObject* groundObject = GameObject::Create("Ground");
	groundObject->_transform.SetPosition(0.0f, 0.0f, 0.0f);
	groundObject->_transform.SetRotation(90.0f, 0.0f, 0.0f);
	groundObject->_transform.SetScale(10.0f, 10.0f, 10.0f);
	Sprite* groundSprite = groundObject->AddComponent<Sprite>();
	groundSprite->SetTexture("Assets/Textures/Tiles/grass.png");
	groundObject->AddComponent<SpriteRenderer>();

	Engine::Run();
	Engine::Shutdown();

	return 0;
}