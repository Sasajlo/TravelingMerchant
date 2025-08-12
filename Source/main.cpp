#include <iostream>

#include <Core/Core.hpp>
#include <Graphics/Graphics.hpp>
#include <Utils/Utils.hpp>
#include <Game/Game.hpp>

int main()
{
	if (!Engine::Initialize("Traveling Merchant", 800, 600, false)) {
		std::cerr << "Failed to initialize engine!" << std::endl;
		return -1;
	}

	// Create main scene
	SceneManager::AddScene("Main Scene");

	// Create camera object
	GameObject* cameraObject = GameObject::Create("Main Camera");
	Camera* camera = cameraObject->AddComponent<Camera>();
	camera->SetBackgroundColor(Color::GRAY);
	cameraObject->AddComponent<Debug>();

	// Create player object
	GameObject* playerObject = GameObject::Create("Player");
	playerObject->AddComponent<Player>();
	playerObject->transform.SetScale(1.5f, 1.5f, 0.0f);
	playerObject->AddTag("Player");
	Sprite* playerSprite = playerObject->AddComponent<Sprite>();
	playerSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
	playerSprite->SetTexture("Assets/Textures/player.png");
	playerObject->AddComponent<SpriteRenderer>();
	playerObject->AddComponent<Billboard>();
	auto inventory = playerObject->AddComponent<Inventory>();

	camera->LookAt(playerObject); // Set camera to follow player
	camera->Follow(playerObject, {}); // Set camera to follow player

/**************************************************************** WORLD GENERATION ****************************************************************/

	// Create ground
	for (int y = -2; y <= 2; y++) {
		for (int x = -2; x <= 2; x++) {
			int index = (y + 2) * 5 + x + 2;
			std::string name = "Ground " + std::to_string(index);
			GameObject* gameObject = GameObject::Create(name);
			gameObject->transform.SetScale(10.0f, 10.0f, 10.0f);
			gameObject->transform.SetRotation(90.0f, 0.0f, 0.0f);
			gameObject->transform.SetPosition(x * 10.0f, 0.0f, y * 10.0f);
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
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(3.0f, 3.0f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.01f); // Set pivot to center
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
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(2.0f, 2.0f, 0.0f);
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
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(1.5f, 1.5f, 0.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.0f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/berry_bush.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn plant objects
	for (int i = 0; i < 10; i++) {
		std::string name = "Plant " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(0.6f, 0.6f, 0.0f);
		gameObject->AddTag("Plant");
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.0f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/plant.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

/****************************************************************** INVENTORY UI ******************************************************************/
	
	GameObject* inventoryObject = GameObject::Create("Inventory");
	inventoryObject->SetActive(false);
	inventoryObject->transform.SetPosition(Engine::GetWindowSize().width * 0.5f, Engine::GetWindowSize().height * 0.5f, 0.0f);

	// Inventory Equipment
	{
		GameObject* object = inventoryObject->CreateChild("Equipment");
		object->transform.SetPosition(-200.0f, 0.0f, 0.0f);
		auto* image = object->AddComponent<ImageRenderer>();
		image->SetImage("Assets/Textures/inventory_equipment.png");
		image->SetSize(600, 600);
		image->SetPivot(1.0f, 0.5f);
	}

	// Inventory Bag
	{
		GameObject* bagObject = inventoryObject->CreateChild("Bag");
		bagObject->transform.SetPosition(200.0f, 0.0f, 0.0f);
		auto* image= bagObject->AddComponent<ImageRenderer>();
		image->SetImage("Assets/Textures/inventory_container.png");
		image->SetSize(600, 300);
		image->SetPivot(0.0f, 0.5f);
	}

/****************************************************************** STATS UI ******************************************************************/

	// Level bar
	{
		GameObject* object = GameObject::Create("Level Text");
		object->transform.SetPosition(20.0f, 20.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 48.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Level: 1   XP: 0/100");
	}

	// Health bar
	{
		GameObject* object = GameObject::Create("Health Text");
		object->transform.SetPosition(20.0f, 70.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 48.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Health: 100/100");
	}

	// Mana bar
	{
		GameObject* object = GameObject::Create("Mana Text");
		object->transform.SetPosition(20.0f, 120.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 48.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Mana: 100/100");
	}

	// Hunger bar
	{
		GameObject* object = GameObject::Create("Hunger Text");
		object->transform.SetPosition(20.0f, 170.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 48.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Hunger: 100/100");
	}

/****************************************************************** TEXT UI ******************************************************************/

	// UI text overlay
	GameObject* cursorTextObject = GameObject::Create("Cursor Text");
	cursorTextObject->transform.SetPosition(20.0f, 40.0f, 0.0f);
	auto* cursorText = cursorTextObject->AddComponent<TextRenderer>();
	cursorText->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 36.0f);
	cursorText->SetPivot(0.0f, 1.0f);
	cursorText->SetActive(false); // Initially hidden

	// UI text overlay
	GameObject* fpsTextObject = GameObject::Create("FPS Text");
	fpsTextObject->transform.SetPosition(Engine::GetWindowSize().width - 20.0f, 20.0f, 0.0f);
	auto* fpsText = fpsTextObject->AddComponent<TextRenderer>();
	fpsText->SetFont("Assets/Fonts/LibertinusSerif-Regular.ttf", 48.0f);
	fpsText->SetPivot(1.0f, 0.0f);

	Engine::Run();
	Engine::Shutdown();

	return 0;
}