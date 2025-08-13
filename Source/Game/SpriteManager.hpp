#pragma once

#include <iostream>
#include <algorithm>
#include <Core/GameObject.hpp>
#include <Graphics/Graphics.hpp>
#include <Utils/Utils.hpp>

namespace TM
{
    namespace Game
    {
        class SpriteManager : public Component
        {
        private:
            Sprite* _sprite = nullptr;
            Camera* _camera = nullptr;

            const int DOWN = 0;
            const int UP = 1;
            const int RIGHT = 2;
            const int LEFT = 3;

            int _lastDirection = DOWN;

        public:
            SpriteManager(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
                _sprite = _gameObject.GetComponent<Sprite>();
                _camera = Camera::GetMain();
            }

            void Start() override
            {
                SetIdleAnimation();
            }

            void SetIdleAnimation()
            {
                _sprite->SetSpriteSheet("Assets/Textures/Player/Animations/idle.png", 4, 4);
                _sprite->SetFrameRate(5);
            }

            void SetRunAnimation()
            {
                _sprite->SetSpriteSheet("Assets/Textures/Player/Animations/run.png", 6, 4);
                _sprite->SetFrameRate(8);
            }

            void UpdateDirection(Vector3 playerDirection)
            {
                Vector3 playerPosition = _gameObject.transform.position;
                Vector3 cameraPosition = _camera->GetGameObject().transform.position;
                Vector3 toPlayer = playerPosition - cameraPosition; 
                toPlayer.y = 0;
                toPlayer = toPlayer.Normalized();

                // Get angle in radians
                float angleRad = acos(std::clamp(toPlayer * playerDirection, -1.0f, 1.0f));

                // Convert to degrees if needed
                float angleDeg = glm::degrees(angleRad);
                //std::cout << angleDeg << std::endl;

                int newDirection = -1;

                if (angleDeg < 45) newDirection = UP;
                else if (angleDeg > 135) newDirection = DOWN;
                else {
                    float sign = playerDirection.Cross(toPlayer).y;
                    if (sign < 0) newDirection = LEFT;
                    else newDirection = RIGHT;
                }

                if (newDirection != -1 && newDirection != _lastDirection) {
                    _sprite->SetAnimationOffset(newDirection);
                    _lastDirection = newDirection;
                }

            }
        };
    }
}