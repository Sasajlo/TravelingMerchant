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
	camera->SetBackgroundColor(Color( 100, 118, 69 ));
	cameraObject->AddComponent<Debug>();

	// Create player object
	GameObject* playerObject = GameObject::Create("Player");
	playerObject->AddComponent<Player>();
	playerObject->transform.SetScale(2.0f, 2.0f, 2.0f);
	playerObject->AddTag("Player");
	Sprite* playerSprite = playerObject->AddComponent<Sprite>();
	playerSprite->SetPivot(0.5f, 0.35f); // Set pivot to center
	//playerSprite->SetTexture("Assets/Textures/player.png");
	playerObject->AddComponent<SpriteRenderer>();
	playerObject->AddComponent<Billboard>();
	playerObject->AddComponent<SpriteManager>();
	auto inventory = playerObject->AddComponent<Inventory>();

	camera->LookAt(playerObject); // Set camera to follow player
	camera->Follow(playerObject, {}); // Set camera to follow player

/**************************************************************** ENEMIES ****************************************************************/

	// Spawn slimes
	for (int i = 0; i < 10; i++) {
		GameObject* slimeObject = GameObject::Create("Slime " + std::to_string(i));
		slimeObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		slimeObject->transform.SetScale(2.0f, 2.0f, 2.0f);
		slimeObject->AddTag("Enemy");
		slimeObject->AddTag("Slime");
		Sprite* slimeSprite = slimeObject->AddComponent<Sprite>();
		slimeSprite->SetPivot(0.5f, 0.5f); // Set pivot to center
		slimeObject->AddComponent<Slime>();
		slimeObject->AddComponent<SpriteRenderer>();
		slimeObject->AddComponent<Billboard>();
		auto _spriteManager = slimeObject->AddComponent<SpriteManager>();
		_spriteManager->AddAnimationState("idle", "Assets/Textures/Mobs/Animations/Slime/slime_idle.png", 4, 4, 5.5f);
		_spriteManager->AddAnimationState("move", "Assets/Textures/Mobs/Animations/Slime/slime_idle.png", 4, 4, 7.0f);
		_spriteManager->AddAnimationState("death", "Assets/Textures/Mobs/Animations/Slime/slime_death.png", 5, 1, 10.0f);
	}

/**************************************************************** WORLD GENERATION ****************************************************************/

	// Create ground
	for (int y = -2; y <= 2; y++) {
		for (int x = -2; x <= 2; x++) {
			int index = (y + 2) * 5 + x + 2;
			std::string name = "Ground " + std::to_string(index);
			GameObject* gameObject = GameObject::Create(name);
			gameObject->transform.SetScale(10.0f, 10.0f, 10.0f);
			gameObject->transform.SetRotation(-90.0f, 0.0f, 0.0f);
			gameObject->transform.SetPosition(x * 10.0f, 0.0f, y * 10.0f);
			Sprite* sprite = gameObject->AddComponent<Sprite>();
			sprite->SetTexture("Assets/Textures/ground.png");
			gameObject->AddComponent<SpriteRenderer>();
			gameObject->AddTag("Ground");
		}
	}

	// Spawn tree objects
	for (int i = 0; i < 30; i++) {
		std::string name = "Tree " + std::to_string(i);
		GameObject* treeObject = GameObject::Create(name);
		treeObject->AddTag("Tree");
		treeObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		treeObject->transform.SetScale(3.5f, 3.5f, 3.5f);
		Sprite* baseSprite = treeObject->AddComponent<Sprite>();
		baseSprite->SetPivot(0.5f, 0.08f); // Set pivot to center
		baseSprite->SetTexture("Assets/Textures/tree.png");
		auto baseRenderer = treeObject->AddComponent<SpriteRenderer>();
		baseRenderer->EnableSwaying(true);
		baseRenderer->SetSwayAmount(0.005f);  // How much it sways
		baseRenderer->SetSwaySpeed(1.6f);    // Speed of swaying
		treeObject->AddComponent<Billboard>();
		treeObject->AddComponent<Interactable>();

		// Create tree top
		{
			GameObject* treeTopObject = treeObject->CreateChild(name + " Top");
			treeTopObject->AddTag("Tree");
			treeTopObject->transform.SetPosition(0.0f, 0.225f, 0.0001f);
			Sprite* treeTopSprite = treeTopObject->AddComponent<Sprite>();
			treeTopSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
			treeTopSprite->SetTexture("Assets/Textures/tree_top.png");
			auto treeTopRenderer = treeTopObject->AddComponent<SpriteRenderer>();
			treeTopRenderer->EnableSwaying(true);
			treeTopRenderer->SetSwayAmount(0.03f);  // How much it sways
			treeTopRenderer->SetSwaySpeed(1.6f);    // Speed of swaying
			//gameObject->AddComponent<Billboard>();
			//treeTopObject->AddComponent<Interactable>();
		}
	}

	// Spawn stone objects
	for (int i = 0; i < 20; i++) {
		std::string name = "Stone " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->AddTag("Stone");
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(1.3f, 1.3f, 1.3f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.15f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/stone.png");
		gameObject->AddComponent<SpriteRenderer>();
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn berry bush objects
	for (int i = 0; i < 20; i++) {
		std::string name = "Berry Bush " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->AddTag("Berry Bush");
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(1.0f, 1.0f, 1.0f);
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.1f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/berry_bush.png");
		auto renderer = gameObject->AddComponent<SpriteRenderer>();
		// Enable swaying with custom parameters
		renderer->EnableSwaying(true);
		renderer->SetSwayAmount(0.05f);  // How much it sways
		renderer->SetSwaySpeed(1.6f);    // Speed of swaying
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

	// Spawn plant objects
	for (int i = 0; i < 20; i++) {
		std::string name = "Plant " + std::to_string(i);
		GameObject* gameObject = GameObject::Create(name);
		gameObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		gameObject->transform.SetScale(0.6f, 0.6f, 0.6f);
		gameObject->AddTag("Plant");
		Sprite* sprite = gameObject->AddComponent<Sprite>();
		sprite->SetPivot(0.5f, 0.35f); // Set pivot to center
		sprite->SetTexture("Assets/Textures/plant.png");
		auto renderer = gameObject->AddComponent<SpriteRenderer>();
		// Enable swaying with custom parameters
		renderer->EnableSwaying(true);
		renderer->SetSwayAmount(0.06f);  // How much it sways
		renderer->SetSwaySpeed(1.6f);    // Speed of swaying
		gameObject->AddComponent<Billboard>();
		gameObject->AddComponent<Interactable>();
	}

/****************************************************************** INVENTORY UI ******************************************************************/
	
	GameObject* inventoryObject = GameObject::Create("Inventory");
	inventoryObject->transform.SetPosition(Engine::GetWindowSize().width * 0.5f, Engine::GetWindowSize().height * 0.5f, 0.0f);

	// Inventory Equipment
	{
		GameObject* equipmentObject = inventoryObject->CreateChild("Equipment");
		equipmentObject->transform.SetPosition(-Engine::GetWindowSize().width * 0.5f + 350.0f, Engine::GetWindowSize().height * 0.5f - 400.0f, 0.0f);
		auto* image = equipmentObject->AddComponent<ImageRenderer>();
		image->SetImage("Assets/Textures/inventory_equipment.png");
		image->SetSize(600, 600);
		image->SetPivot(0.5f, 0.5f);
		equipmentObject->SetActive(false);

		// Create title
		{
			GameObject* titleObject = equipmentObject->CreateChild("Equipment Title");
			titleObject->transform.SetPosition(0.0f, -300.0f, 0.0f);
			auto* titleBackground = titleObject->AddComponent<ImageRenderer>();
			titleBackground->SetImage("Assets/Textures/title_frame.png");
			titleBackground->SetSize(320, 80);
			titleBackground->SetPivot(0.5f, 0.5f);
			auto* titleText = titleObject->AddComponent<TextRenderer>();
			titleText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 22.0f);
			titleText->SetText("Equipment");
			titleText->SetPivot(0.5f, 0.5f);
		}
	}

	// Inventory Bag
	{
		GameObject* bagObject = inventoryObject->CreateChild("Bag");
		bagObject->transform.SetPosition(Engine::GetWindowSize().width * 0.5f - 350.0f, Engine::GetWindowSize().height * 0.5f - 250.0f, 0.0f);
		auto* image= bagObject->AddComponent<ImageRenderer>();
		image->SetImage("Assets/Textures/inventory_container.png");
		image->SetSize(600, 300);
		image->SetPivot(0.5f, 0.5f);
		bagObject->SetActive(false);

		// Create title
		{
			GameObject* titleObject = bagObject->CreateChild("Bag Title");
			titleObject->transform.SetPosition(0.0f, -150.0f, 0.0f);
			auto* titleBackground = titleObject->AddComponent<ImageRenderer>();
			titleBackground->SetImage("Assets/Textures/title_frame.png");
			titleBackground->SetSize(280, 70);
			titleBackground->SetPivot(0.5f, 0.5f);
			auto* titleText = titleObject->AddComponent<TextRenderer>();
			titleText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 22.0f);
			titleText->SetText("Bag");
			titleText->SetPivot(0.5f, 0.5f);
		}
	}

/****************************************************************** BUTTONS ******************************************************************/

// Bag Button
{
	GameObject* inventoryButtonObject = GameObject::Create("Bag Button");
	inventoryButtonObject->transform.SetPosition(Engine::GetWindowSize().width - 20.0f, Engine::GetWindowSize().height - 10.f, 0.0f);
	auto* inventoryButtonImage = inventoryButtonObject->AddComponent<ImageRenderer>();
	inventoryButtonImage->SetImage("Assets/Textures/Buttons/bag_button.png");
	inventoryButtonImage->SetSize(75, 75);
	inventoryButtonImage->SetPivot(1.0f, 1.0f);

	auto* buttonText = inventoryButtonObject->AddComponent<TextRenderer>();
	buttonText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 16.0f);
	buttonText->SetText("B");
	buttonText->SetPivot(1.0f, 1.0f);
}

// Equipment Button
{
	GameObject* inventoryButtonObject = GameObject::Create("Equipment Button");
	inventoryButtonObject->transform.SetPosition(Engine::GetWindowSize().width - 100.0f, Engine::GetWindowSize().height - 10.f, 0.0f);
	auto* inventoryButtonImage = inventoryButtonObject->AddComponent<ImageRenderer>();
	inventoryButtonImage->SetImage("Assets/Textures/Buttons/equipment_button.png");
	inventoryButtonImage->SetSize(75, 75);
	inventoryButtonImage->SetPivot(1.0f, 1.0f);

	auto* buttonText = inventoryButtonObject->AddComponent<TextRenderer>();
	buttonText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 16.0f);
	buttonText->SetText("H");
	buttonText->SetPivot(1.0f, 1.0f);
}

/****************************************************************** STATS UI ******************************************************************/

	// Level bar
	{
		GameObject* object = GameObject::Create("Level Text");
		object->transform.SetPosition(20.0f, 20.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 18.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Level: 1   XP: 0/100");
	}

	// Health bar
	{
		GameObject* object = GameObject::Create("Health Text");
		object->transform.SetPosition(20.0f, 50.0f, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 18.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Health: 100/100");
	}

	// Mana bar
	{
		GameObject* object = GameObject::Create("Mana Text");
		object->transform.SetPosition(20.0f, 80, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 18.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Mana: 100/100");
	}

	// Hunger bar
	{
		GameObject* object = GameObject::Create("Hunger Text");
		object->transform.SetPosition(20.0f, 110, 0.0f);
		auto* text = object->AddComponent<TextRenderer>();
		text->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 18.0f);
		text->SetPivot(0.0f, 0.0f);
		text->SetText("Hunger: 100/100");
	}

/****************************************************************** TEXT UI ******************************************************************/

	// UI text overlay
	GameObject* cursorTextObject = GameObject::Create("Cursor Text");
	cursorTextObject->transform.SetPosition(20.0f, 40.0f, 0.0f);
	auto* cursorText = cursorTextObject->AddComponent<TextRenderer>();
	cursorText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 18.0f);
	cursorText->SetPivot(0.0f, 1.0f);
	cursorText->SetActive(false); // Initially hidden

	// UI text overlay
	GameObject* fpsTextObject = GameObject::Create("FPS Text");
	fpsTextObject->transform.SetPosition(Engine::GetWindowSize().width - 20.0f, 20.0f, 0.0f);
	auto* fpsText = fpsTextObject->AddComponent<TextRenderer>();
	fpsText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 22.0f);
	fpsText->SetPivot(1.0f, 0.0f);

	Engine::Run();
	Engine::Shutdown();

	return 0;
}