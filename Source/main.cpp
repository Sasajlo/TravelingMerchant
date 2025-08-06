#include <iostream>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Core/GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Color.hpp>
#include <Game/Player.hpp>

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
	playerSprite->SetTexture("Assets/Textures/Tiles/bedrock.png");
	playerObject->AddComponent<SpriteRenderer>();

	camera->LookAt(playerObject); // Set camera to follow player
	camera->Follow(playerObject, { 0.0f, 5.0f, 10.0f }); // Set camera to follow player

	// Create ground object
	GameObject* groundObject = GameObject::Create("Ground");
	groundObject->_transform.SetPosition(0.0f, -5.0f, 0.0f);
	groundObject->_transform.SetRotation(90.0f, 0.0f, 0.0f);
	groundObject->_transform.SetScale(10.0f, 10.0f, 10.0f);
	Sprite* groundSprite = groundObject->AddComponent<Sprite>();
	groundSprite->SetTexture("Assets/Textures/Tiles/grass.png");
	groundObject->AddComponent<SpriteRenderer>();

	Engine::Run();
	Engine::Shutdown();

	return 0;
}