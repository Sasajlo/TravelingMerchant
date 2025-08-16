#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Core/SceneManager.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Math.hpp>
#include <Game/Billboard.hpp>
#include <Game/Inventory/Inventory.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class Interactable : public Component
        {
        private:
            bool _hovered = false;
            SpriteRenderer* _spriteRenderer;

            void SpawnLogs()
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
                    sprite->SetPivot(0.5f, 0.1f); // Set pivot to center
                    logObject->AddComponent<SpriteRenderer>();
                    logObject->AddComponent<Billboard>();
                    logObject->AddComponent<Interactable>();
                }
            }

            void SpawnRocks()
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

        public:
            Interactable(GameObject& gameObject) : Component(gameObject) {}

            void SetHovered(bool h) { _hovered = h; if (_spriteRenderer) _spriteRenderer->SetHovered(h); }
            bool IsHovered() const { return _hovered; }

            void Awake() override
            {
                _spriteRenderer = _gameObject.GetComponent<SpriteRenderer>();
            }

            void Interact(GameObject* player) 
            {
                if (_gameObject.HasTag("Tree"))
                {
                    SpawnLogs();
                }
                else if (_gameObject.HasTag("Stone"))
                {
                    SpawnRocks();
                }
                else if (_gameObject.HasTag("Berry Bush"))
                {
					_gameObject.GetComponent<Sprite>()->SetTexture("Assets/Textures/berry_bush_empty.png");
                    _gameObject.RemoveComponent<Interactable>();
                    player->GetComponent<Inventory>()->AddItem({ "Berries", "Assets/Textures/Icons/berries_icon.png", 1 });
                }
                else if (_gameObject.HasTag("Plant"))
                {
                    player->GetComponent<Inventory>()->AddItem({ "Herbs", "Assets/Textures/Icons/herbs_icon.png", 1 });
                }
                else if (_gameObject.HasTag("Log"))
                {
                    player->GetComponent<Inventory>()->AddItem({ "Log", "Assets/Textures/Icons/log_icon.png", 1 });
                }
                else if (_gameObject.HasTag("Rocks"))
                {
                    player->GetComponent<Inventory>()->AddItem({ "Rocks", "Assets/Textures/Icons/rocks_icon.png", 1 });
                }


                if (!_gameObject.HasTag("Berry Bush"))
                {
                    SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
                }
            }

            std::string GetInteractionText()
            {
                if (_gameObject.HasTag("Tree"))
                {
                    return "Chop Tree";
                }
                else if (_gameObject.HasTag("Log"))
                {
                    return "Collect Log";
                }
                else if (_gameObject.HasTag("Stone"))
                {
                    return "Mine Stone";
                }
                else if (_gameObject.HasTag("Rocks"))
                {
                    return "Collect Rocks";
                }
                else if (_gameObject.HasTag("Berry Bush"))
                {
                    return "Harvest Berries";
                }
                else if (_gameObject.HasTag("Plant"))
                {
                    return "Collect Plant";
                }

                return "Interact with " + _gameObject.GetName();
            }
        };
    }
}