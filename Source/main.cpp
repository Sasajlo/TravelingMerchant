#include <iostream>

#include <Core/Core.hpp>
#include <Graphics/Graphics.hpp>
#include <Utils/Utils.hpp>
#include <Game/Game.hpp>
#include "Audio/AudioSource.hpp"

using namespace TM::Audio;

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
	playerObject->AddComponent<PlayerStats>();
	playerObject->AddComponent<PlayerHUD>();
	playerObject->AddComponent<Inventory>();

	auto mapIcon = playerObject->AddComponent<MapIcon>();
	mapIcon->SetIcon("Assets/Textures/MapIcons/player_icon.png");
	mapIcon->SetPivot(0.5f, 0.5f);
	mapIcon->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	mapIcon->SetAlwaysOnTop(true);

	// Create camera target object
	GameObject* cameraTargetObject = GameObject::Create("Camera Target");
	cameraTargetObject->transform.SetPosition(0.0f, 0.5f, 0.0f); // Slightly above player
	auto cameraTarget = cameraTargetObject->AddComponent<CameraTarget>();
	cameraTarget->SetTarget(playerObject);

	camera->LookAt(cameraTargetObject); // Set camera to follow player
	camera->Follow(cameraTargetObject, {}); // Set camera to follow player

	auto* playerAudoSource = playerObject->AddComponent<AudioSource>();
	playerAudoSource->SetAudioType(AudioSource::AudioType::ThreeDimensional);
	playerAudoSource->SetVolume(5.0f);
	playerAudoSource->SetMinDistance(0.1f);  // Much closer
	playerAudoSource->SetMaxDistance(500.0f); // Much closer
	playerAudoSource->SetRolloffFactor(1.0f);
	playerAudoSource->SetReferenceDistance(10.0f);

/****************************************************************** AUDIO SETUP ******************************************************************/

	// Initialize audio system
	if (!AudioSource::InitializeAudioSystem()) {
		std::cerr << "Failed to initialize audio system!" << std::endl;
	}

	//// Add background music (2D audio)
	//GameObject* musicObject = GameObject::Create("Background Music");
	//auto* backgroundMusic = musicObject->AddComponent<TM::Audio::AudioSource>();
	//backgroundMusic->SetAudioType(TM::Audio::AudioSource::AudioType::TwoDimensional);
	//backgroundMusic->SetAudioFile("Assets/Sounds/ambient_music.wav");
	//backgroundMusic->SetVolume(0.3f);
	//backgroundMusic->SetLooping(true);
	//backgroundMusic->Play();

	//// Add player footsteps (3D audio)
	//auto* playerFootsteps = playerObject->AddComponent<TM::Audio::AudioSource>();
	//playerFootsteps->SetAudioType(TM::Audio::AudioSource::AudioType::ThreeDimensional);
	//playerFootsteps->SetAudioFile("Assets/Sounds/footsteps.wav");
	//playerFootsteps->SetVolume(0.7f);
	//playerFootsteps->SetMinDistance(1.0f);
	//playerFootsteps->SetMaxDistance(20.0f);
	//playerFootsteps->SetRolloffFactor(1.0f);
	//playerFootsteps->SetReferenceDistance(1.0f);

	//// Add UI sound effects (2D audio)
	//GameObject* uiAudioObject = GameObject::Create("UI Audio");
	//auto* uiAudio = uiAudioObject->AddComponent<TM::Audio::AudioSource>();
	//uiAudio->SetAudioType(TM::Audio::AudioSource::AudioType::TwoDimensional);
	//uiAudio->SetAudioFile("Assets/Sounds/button_click.wav");
	//uiAudio->SetVolume(0.8f);
	//uiAudio->SetLooping(false);

/***************************************************************** NPC *****************************************************************/

	// Spawn tutorial NPC
	{
		GameObject* npcObject = GameObject::Create("Tutorial NPC");
		npcObject->transform.SetPosition(Math::RandomFloat(-20.0f, 20.f), 0.0f, Math::RandomFloat(-20.0f, 20.f));
		npcObject->transform.SetScale(2.0f, 2.0f, 2.0f);
		npcObject->AddTag("NPC");
		Sprite* npcSprite = npcObject->AddComponent<Sprite>();
		npcSprite->SetPivot(0.5f, 0.35f); // Set pivot to center
		npcSprite->SetSpriteSheet("Assets/Textures/Mobs/NPCs/NPC1/idle.png", 2, 4);
		npcSprite->SetFrameRate(5.0f);
		npcObject->AddComponent<SpriteRenderer>();
		npcObject->AddComponent<SpriteManager>();
		npcObject->AddComponent<Billboard>();
		npcObject->AddComponent<Interactable>();
		auto mapIcon = npcObject->AddComponent<MapIcon>();
		mapIcon->SetIcon("Assets/Textures/Miscellaneous/question_mark.png");
		mapIcon->SetPivot(0.5f, 0.5f);
		mapIcon->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
		mapIcon->SetAlwaysOnTop(true);

		// Add question mark above NPC
		{
			GameObject* questionMarkObject = npcObject->CreateChild("Question Mark");
			questionMarkObject->transform.SetPosition(0.0f, 0.5f, 0.0001f);
			questionMarkObject->transform.SetScale(0.25f, 0.25f, 0.25f);
			Sprite* questionMarkSprite = questionMarkObject->AddComponent<Sprite>();
			questionMarkSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
			questionMarkSprite->SetTexture("Assets/Textures/Miscellaneous/question_mark.png", TextureFilter::LINEAR);
			questionMarkSprite->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
			questionMarkObject->AddComponent<SpriteRenderer>();
		}
	}

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
		slimeObject->AddComponent<SpriteManager>();
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

			auto mapIcon = gameObject->AddComponent<MapIcon>();
			mapIcon->SetIcon("Assets/Textures/ground.png");
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
		auto mapIcon = treeObject->AddComponent<MapIcon>();
		mapIcon->SetIcon("Assets/Textures/MapIcons/tree.png");
		mapIcon->SetPivot(0.5f, 0.08f);

		// Create tree top
		{
			GameObject* treeTopObject = treeObject->CreateChild(name + " Top");
			treeTopObject->AddTag("Tree");
			treeTopObject->transform.SetPosition(0.0f, 0.133f, 0.0001f);
			Sprite* treeTopSprite = treeTopObject->AddComponent<Sprite>();
			treeTopSprite->SetPivot(0.5f, 0.0f); // Set pivot to center
			treeTopSprite->SetTexture("Assets/Textures/tree_top.png");
			auto treeTopRenderer = treeTopObject->AddComponent<SpriteRenderer>();
			treeTopRenderer->EnableSwaying(true);
			treeTopRenderer->SetSwayAmount(0.03f);  // How much it sways
			treeTopRenderer->SetSwaySpeed(1.6f);    // Speed of swaying
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
		auto mapIcon = gameObject->AddComponent<MapIcon>();
		mapIcon->SetIcon("Assets/Textures/MapIcons/stone_icon.png");
		mapIcon->SetPivot(0.5f, 0.15f);
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
		auto mapIcon = gameObject->AddComponent<MapIcon>();
		mapIcon->SetIcon("Assets/Textures/berry_bush.png");
		mapIcon->SetPivot(0.5f, 0.1f);
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
		auto mapIcon = gameObject->AddComponent<MapIcon>();
		mapIcon->SetIcon("Assets/Textures/plant.png");
		mapIcon->SetPivot(0.5f, 0.35f);
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
	fpsTextObject->transform.SetPosition(Engine::GetWindowSize().width - 220.0f, 20.0f, 0.0f);
	auto* fpsText = fpsTextObject->AddComponent<TextRenderer>();
	fpsText->SetFont("Assets/Fonts/ManaSeedTitle.ttf", 22.0f);
	fpsText->SetPivot(1.0f, 0.0f);

	Engine::Run();

	// Shutdown engine first so all components are destroyed before tearing down audio
	Engine::Shutdown();

	// Then cleanup audio system once sources/buffers are gone
	TM::Audio::AudioSource::CleanupAudioSystem();

	return 0;
}