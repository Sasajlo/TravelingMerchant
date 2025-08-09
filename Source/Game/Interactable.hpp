#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Utils/Math.hpp>
#include <Game/Billboard.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Game;

namespace TM
{
    namespace Game
    {
        class Interactable : public Component
        {
        public:
            Interactable(GameObject& gameObject) : Component(gameObject) {}

            void Interact() 
            {
                if (_gameObject.HasTag("Tree"))
                {
                    SpawnLogs();
                }
                else if (_gameObject.HasTag("Stone"))
                {
                    SpawnRocks();
                }

                SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
            }

            void SpawnLogs()
            {
                // Example logic to spawn logs
                for (int i = 0; i < 3; ++i)
                {
                    std::string logName = _gameObject.GetName() + " Log " + std::to_string(i);
                    GameObject* logObject = GameObject::Create(logName);
                    logObject->_transform.SetPosition(_gameObject._transform.GetPosition() + Vector3(Math::RandomFloat(-1.0f, 1.0f), 0.0f, Math::RandomFloat(-1.0f, 1.0f)));
                    logObject->_transform.SetScale(0.6f, 0.6f, 0.0f);
                    Sprite* sprite = logObject->AddComponent<Sprite>();
                    sprite->SetTexture("Assets/Textures/log.png");
                    sprite->SetPivot(0.5f, 0.0f); // Set pivot to center
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
                    rocksObject->_transform.SetPosition(_gameObject._transform.GetPosition() + Vector3(Math::RandomFloat(-1.0f, 1.0f), 0.0f, Math::RandomFloat(-1.0f, 1.0f)));
                    rocksObject->_transform.SetScale(0.6f, 0.6f, 0.0f);
                    Sprite* sprite = rocksObject->AddComponent<Sprite>();
                    sprite->SetTexture("Assets/Textures/rocks.png");
					sprite->SetPivot(0.5f, 0.0f); // Set pivot to center
                    rocksObject->AddComponent<SpriteRenderer>();
                    rocksObject->AddComponent<Billboard>();
                    rocksObject->AddComponent<Interactable>();
                }
            }
        };
    }
}