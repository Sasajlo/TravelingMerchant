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
			Vector3 _cameraOffset = Vector3(0.0f, 10.0f, 10.0f); // Camera offset from player

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
				// Handle camera rotation
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

				// Handle player movement
                Vector3 direction(0.0f, 0.0f, 0.0f);

                if (Input::IsKeyHeld(GLFW_KEY_W))
                    direction.z -= 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_S))
                    direction.z += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_D))
                    direction.x += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_A))
                    direction.x -= 1.0f;

                if (direction.Length() > 0.0f)
                    direction = direction.Normalized();

                float radians = glm::radians(-_cameraAngle);
                float sinA = std::sin(radians);
                float cosA = std::cos(radians);

                Vector3 rotatedDir;
                rotatedDir.x = direction.x * cosA - direction.z * sinA;
                rotatedDir.z = direction.x * sinA + direction.z * cosA;
                rotatedDir.y = 0.0f; // No vertical movement

                _gameObject._transform._position += rotatedDir * SPEED * deltaTime;
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