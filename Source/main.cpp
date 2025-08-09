#include <iostream>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Core/GameObject.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Color.hpp>
#include <Utils/Math.hpp>
#include <Game/Player.hpp>
#include <Game/Billboard.hpp>
#include <Game/Interactable.hpp>

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
	playerObject->_transform.SetScale(1.5f, 1.5f, 0.0f);
	Sprite* playerSprite = playerObject->AddComponent<Sprite>();
	playerSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
	playerSprite->SetTexture("Assets/Textures/player.png");
	playerObject->AddComponent<SpriteRenderer>();
	playerObject->AddComponent<Billboard>();

	camera->LookAt(playerObject); // Set camera to follow player
	camera->Follow(playerObject, {}); // Set camera to follow player

	// Create ground
	for (int y = -2; y <= 2; y++) {
		for (int x = -2; x <= 2; x++) {
			int index = (y + 2) * 5 + x + 2;
			std::string name = "Ground " + std::to_string(index);
			GameObject* gameObject = GameObject::Create(name);
			gameObject->_transform.SetScale(10.0f, 10.0f, 10.0f);
			gameObject->_transform.SetRotation(90.0f, 0.0f, 0.0f);
			gameObject->_transform.SetPosition(x * 10.0f, 0.0f, y * 10.0f);
			Sprite* sprite = gameObject->AddComponent<Sprite>();
			sprite->SetTexture("Assets/Textures/ground.png");
			gameObject->AddComponent<SpriteRenderer>();
			gameObject->AddTag("Ground");
		}
	}

	// Spawn tree objects
	for (int i = 0; i < 20; i++) {
		std::string name = "Tree " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->AddTag("Tree");
		gameObject->_transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->_transform.SetScale(3.0f, 3.0f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.03f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/tree.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn stone objects
	for (int i = 0; i < 10; i++) {
		std::string name = "Stone " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->AddTag("Stone");
		gameObject->_transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->_transform.SetScale(2.0f, 2.0f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.1f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/stone.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn berry bush objects
	for (int i = 0; i < 10; i++) {
		std::string name = "Berry Bush " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->AddTag("Berry Bush");
		gameObject->_transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->_transform.SetScale(1.5f, 1.5f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.1f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/berry_bush.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn plant objects
	for (int i = 0; i < 10; i++) {
		std::string name = "Plant " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->_transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->_transform.SetScale(0.5f, 0.5f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.1f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/plant.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	Engine::Run();
	Engine::Shutdown();

	return 0;
}