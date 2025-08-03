#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Core/Input.hpp>

using namespace TM::Core;

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
                    std::cout << "Player position: " <<
                        _gameObject._transform._position.x << " " <<
                        _gameObject._transform._position.y << " " <<
                        _gameObject._transform._position.z << std::endl;
                }

                if (Input::IsKeyHeld(GLFW_KEY_S))
                {
                    _gameObject._transform._position.y -= SPEED * deltaTime;
                    std::cout << "Player position: " <<
                        _gameObject._transform._position.x << " " <<
                        _gameObject._transform._position.y << " " <<
                        _gameObject._transform._position.z << std::endl;
                }

                if (Input::IsKeyHeld(GLFW_KEY_D))
                {
                    _gameObject._transform._position.x += SPEED * deltaTime;
                    std::cout << "Player position: " <<
                        _gameObject._transform._position.x << " " <<
                        _gameObject._transform._position.y << " " <<
                        _gameObject._transform._position.z << std::endl;
                }

                if (Input::IsKeyHeld(GLFW_KEY_A))
                {
                    _gameObject._transform._position.x -= SPEED * deltaTime;
                    std::cout << "Player position: " <<
                        _gameObject._transform._position.x << " " <<
                        _gameObject._transform._position.y << " " <<
                        _gameObject._transform._position.z << std::endl;
                }
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