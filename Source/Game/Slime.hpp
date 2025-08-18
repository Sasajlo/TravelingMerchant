#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Utils/Vector3.hpp>
#include <Utils/Math.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Utils;

#define MAX_MOVE_DELAY 15.0f
#define MIN_MOVE_DELAY 5.0f
#define MOVE_SPEED 1.0f

namespace TM
{
    namespace Game
    {
        class Slime : public Interactable
        {
        private:
            float _timePassed = 0.0f;
            Vector3 _targetPosition;
            bool _hasTarget = false;

            int _health = 10;
            float deathTimer = 0.5;

            SpriteManager* _spriteManager = nullptr;

        public:
            Slime(GameObject& gameObject) : Interactable(gameObject) {}

            void Awake() override
            {
                Interactable::Awake();
                _spriteManager = _gameObject.GetComponent<SpriteManager>();
                _spriteManager->AddAnimationState("idle", "Assets/Textures/Mobs/Animations/Slime/slime_idle.png", 4, 4, 5.5f, true);
                _spriteManager->AddAnimationState("move", "Assets/Textures/Mobs/Animations/Slime/slime_idle.png", 4, 4, 7.0f, true);
                _spriteManager->AddAnimationState("death", "Assets/Textures/Mobs/Animations/Slime/slime_death.png", 5, 1, 10.0f);
            }

            void Update(float deltaTime) override
            {
                if (_health <= 0)
                {
                    deathTimer -= deltaTime;
                    if (deathTimer <= 0) Remove();
                    return;
                }

                if (_hasTarget)
                {
                    Vector3 toTarget = _targetPosition - _gameObject.transform.position;
                    if (toTarget.Length() > 0.1)
                    {
                        toTarget = toTarget.Normalized();
                        _spriteManager->SetDirection(toTarget);
                        _gameObject.transform.position += toTarget * MOVE_SPEED * deltaTime;
                    }
                    else {
                        _hasTarget = false;
                        _spriteManager->PlayAnimation("idle");
                    }
                }

                _timePassed -= deltaTime;
                if (_timePassed <= 0)
                {
                    _targetPosition = _gameObject.transform.position + Vector3(Math::RandomFloat(-5.0f, 5.0f), 0.0f, Math::RandomFloat(-5.0f, 5.0f));
                    _hasTarget = true;
                    _timePassed = Math::RandomFloat(MIN_MOVE_DELAY, MAX_MOVE_DELAY);
                    _spriteManager->PlayAnimation("move");
                }
            }

            void StartInteraction(Player* player) override
            {
                Interactable::StartInteraction(player);
                player->Chop(this);
            }

            void Interact(Player* player) override
            {
                Die();
            }

            std::string GetInteractionText() override
            {
                return "Attack Slime";
            }

            void Die()
            {
                _spriteManager->PlayAnimation("death");
                _health = 0;
            }

            void Remove()
            {
                SceneManager::GetActiveScene()->RemoveGameObject(_gameObject.GetName());
            }
        };
    }
}