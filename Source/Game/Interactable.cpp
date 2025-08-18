#include "Interactable.hpp"
#include <Game/Player.hpp>

using namespace TM::Game;

void Interactable::StartInteraction(Player* player)
{
    _valid = false;

    if (_gameObject.HasTag("Tree"))
    {
        player->Chop(this);
    }
    else if (_gameObject.HasTag("Stone"))
    {
        player->Mine(this);
    }
    else if (_gameObject.HasTag("Berry Bush"))
    {
        player->Harvest(this);
    }
    else if (_gameObject.HasTag("Plant"))
    {
        player->Harvest(this);
    }
    else if (_gameObject.HasTag("Log"))
    {
        player->Harvest(this);
    }
    else if (_gameObject.HasTag("Rocks"))
    {
        player->Harvest(this);
    }
}

void Interactable::Interact(Player* player)
{
    if (_gameObject.HasTag("Tree"))
    {
        SpawnLogs();
        SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
    }
    else if (_gameObject.HasTag("Stone"))
    {
        SpawnRocks();
        SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
    }
    else if (_gameObject.HasTag("Berry Bush"))
    {
        _gameObject.GetComponent<Sprite>()->SetTexture("Assets/Textures/berry_bush_empty.png");
        _gameObject.RemoveComponent<Interactable>();
        player->AddItem({"Berries", "Assets/Textures/Icons/berries_icon.png", 1});
    }
    else if (_gameObject.HasTag("Plant"))
    {
        player->AddItem({ "Herbs", "Assets/Textures/Icons/herbs_icon.png", 1 });
        SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
    }
    else if (_gameObject.HasTag("Log"))
    {
        player->AddItem({ "Log", "Assets/Textures/Icons/log_icon.png", 1 });
        SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
    }
    else if (_gameObject.HasTag("Rocks"))
    {
        player->AddItem({ "Rocks", "Assets/Textures/Icons/rocks_icon.png", 1 });
        SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
    }
}

void Interactable::SpawnLogs()
{
    // Example logic to spawn logs
    for (int i = 0; i < 3; ++i)
    {
        std::string logName = _gameObject.GetName() + " Log " + std::to_string(i);
        GameObject* logObject = GameObject::Create(logName);
        logObject->transform.SetPosition(_gameObject.transform.GetPosition() + Vector3(Math::RandomFloat(-1.0f, 1.0f), 0.0f, Math::RandomFloat(-1.0f, 1.0f)));
        logObject->transform.SetScale(0.6f, 0.6f, 0.6f);
        logObject->AddTag("Log");
        Sprite* sprite = logObject->AddComponent<Sprite>();
        sprite->SetTexture("Assets/Textures/log.png");
        sprite->SetPivot(0.5f, 0.45); // Set pivot to center
        logObject->AddComponent<SpriteRenderer>();
        logObject->AddComponent<Billboard>();
        logObject->AddComponent<Interactable>();
    }
}

void Interactable::SpawnRocks()
{
    // Example logic to spawn logs
    for (int i = 0; i < 3; ++i)
    {
        std::string logName = _gameObject.GetName() + " Rocks " + std::to_string(i);
        GameObject* rocksObject = GameObject::Create(logName);
        rocksObject->transform.SetPosition(_gameObject.transform.GetPosition() + Vector3(Math::RandomFloat(-1.0f, 1.0f), 0.0f, Math::RandomFloat(-1.0f, 1.0f)));
        rocksObject->transform.SetScale(0.45f, 0.45f, 0.45f);
        rocksObject->AddTag("Rocks");
        Sprite* sprite = rocksObject->AddComponent<Sprite>();
        sprite->SetTexture("Assets/Textures/rocks.png");
        sprite->SetPivot(0.5f, 0.3f); // Set pivot to center
        rocksObject->AddComponent<SpriteRenderer>();
        rocksObject->AddComponent<Billboard>();
        rocksObject->AddComponent<Interactable>();
    }
}
