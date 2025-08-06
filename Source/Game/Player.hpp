#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Core/Input.hpp>
#include <Graphics/Camera.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Core
    {
        class GameObject; // Forward declaration
    }

    namespace Game
    {
        class Player : public Component
        {
        private:
            const float SPEED = 5.0f; // Player movement speed
			float _cameraAngle = 0.0f;
			float _targetAngle = 0.0f;
			Vector3 _cameraOffset = Vector3(0.0f, 5.0f, 10.0f); // Camera offset from player

        public:
			Player(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
                std::cout << "Player awakened!" << std::endl;
            }

            void Start() override
            {
                std::cout << "Player started!" << std::endl;
            }

            void Update(float deltaTime) override
            {
                if (Input::IsKeyPressed(GLFW_KEY_SPACE))
                {
                    std::cout << "Player jumped!" << std::endl;
                }

                if (Input::IsKeyHeld(GLFW_KEY_W))
                {
                    _gameObject._transform._position.y += SPEED * deltaTime;
                }

                if (Input::IsKeyHeld(GLFW_KEY_S))
                {
                    _gameObject._transform._position.y -= SPEED * deltaTime;
                }

                if (Input::IsKeyHeld(GLFW_KEY_D))
                {
                    _gameObject._transform._position.x += SPEED * deltaTime;
                }

                if (Input::IsKeyHeld(GLFW_KEY_A))
                {
                    _gameObject._transform._position.x -= SPEED * deltaTime;
                }

                if (Input::IsKeyPressed(GLFW_KEY_Q))
                {
                    _targetAngle -= 45.0f;
                }

                if (Input::IsKeyPressed(GLFW_KEY_E))
                {
                    _targetAngle += 45.0f;
                }

                // Lerp camera angle
                _cameraAngle = std::lerp(_cameraAngle, _targetAngle, 10.0f * deltaTime);
                Vector3 newOffset = _cameraOffset.RotateAroundY(_cameraAngle);
                Camera::GetMain()->SetFollowOffset(newOffset);
            }

            void Render() override
            {

            }

            void Destroy() override
            {

            }

            void CustomMethod()
            {
                std::cout << "Custom method in Player called" << std::endl;
            }
        };
    }
}