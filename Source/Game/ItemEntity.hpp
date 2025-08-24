#pragma once

#include <iostream>
#include <algorithm>
#include <Core/GameObject.hpp>
#include <Game/Interactable.hpp>
#include <Utils/Math.hpp>

using namespace TM::Core;
using namespace TM::Game;
using namespace TM::Utils;

#define SPEED 2.5f
#define GRAVITY 5.0f
#define DRAG 0.85f

namespace TM
{
    namespace Game
    {
        class ItemEntity : public Interactable
        {
        private:
            Vector3 _velocity;

        public:
            ItemEntity(GameObject& gameObject) : Interactable(gameObject) {}

            void Awake() override
            {
                Interactable::Awake();
                _velocity = {
                    Math::RandomFloat(SPEED * 0.25f, SPEED * 0.5f) * (Math::RandomBool() ? 1 : -1),
                    Math::RandomFloat(SPEED * 0.75f, SPEED),
                    Math::RandomFloat(SPEED * 0.25f, SPEED * 0.5f)  * (Math::RandomBool() ? 1 : -1)
                };
            }

            void Update(float deltaTime) override
            {
                if (Vector3(_velocity.x, 0, _velocity.z).Length() > 0.1f || _gameObject.transform.position.y > 0)
                {
                    // Move object
                    _gameObject.transform.position += _velocity * deltaTime;

                    // Clamp y to 0
                    _gameObject.transform.position.y = std::max(_gameObject.transform.position.y, 0.0f);

                    const bool onGround = _gameObject.transform.position.y == 0.0f;

                    // Apply drag and gravity
                    _velocity -= Vector3(
                        _velocity.x * (onGround ? DRAG * 5 : DRAG) * deltaTime,
                        GRAVITY * deltaTime,
                        _velocity.z * (onGround ? DRAG * 5 : DRAG) * deltaTime
                    );
                }
            }
        };
    }
}