#pragma once

#include <iostream>
#include <algorithm>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Utils/Utils.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class PlayerMovement : public Component
        {
        private:
            const float MAX_SPEED = 2.9f;
            const float CAMERA_ROTATION_SPEED = 5.0f;

            // Movement
            float _speed = 0.0f;
            float _acceleration = 90.0f;
            float _friction = 15.0f;
            Vector3 _velocity = Vector3::Zero;

            Vector3 _inputDirection;
            Vector3 _movementDirection;

            // Camera
            float _cameraAngle = 0.0f;
            float _targetCameraAngle = 0.0f;

            void UpdateCameraRotation(float deltaTime)
            {
                if (Input::IsKeyPressed(GLFW_KEY_Q))
                    _targetCameraAngle -= 45.0f;

                if (Input::IsKeyPressed(GLFW_KEY_E))
                    _targetCameraAngle += 45.0f;

                _cameraAngle = std::lerp(_cameraAngle, _targetCameraAngle,
                    CAMERA_ROTATION_SPEED * deltaTime);
            }

            void UpdateDirection()
            {
                _inputDirection = Vector3::Zero; // Reset input direction

                // WASD input
                if (Input::IsKeyHeld(GLFW_KEY_W))
                    _inputDirection.z -= 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_S))
                    _inputDirection.z += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_D))
                    _inputDirection.x += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_A))
                    _inputDirection.x -= 1.0f;

                if (_inputDirection == Vector3::Zero)
                    return;

                _inputDirection = _inputDirection.Normalized();

                float radians = glm::radians(-_cameraAngle);
                float sinA = std::sin(radians);
                float cosA = std::cos(radians);

                _movementDirection.x = _inputDirection.x * cosA - _inputDirection.z * sinA;
                _movementDirection.z = _inputDirection.x * sinA + _inputDirection.z * cosA;
                _movementDirection.y = 0.0f; // No vertical movement

                _movementDirection = _movementDirection.Normalized();
            }

            void UpdateVelocity(float deltaTime)
            {
                if (HasInput())
                {
                    // Accelerate in movement direction
                    Vector3 impulse = _movementDirection * (_acceleration - _friction) * deltaTime;
                    _velocity += impulse;

                    // Clamp velocity
                    if (_velocity.Length() > MAX_SPEED)
                        _velocity = _velocity.Normalized() * MAX_SPEED;
                }
                else
                {
                    // Apply damping (friction)
                    float damping = 8.0f; // tweak value until it feels right
                    _velocity -= _velocity.Normalized() * 15.0f * deltaTime;

                    // Stop completely when near zero
                    if (_velocity.Length() < 0.05f)
                        _velocity = Vector3::Zero;
                }
            }

            void UpdatePosition(float deltaTime)
            {
                if (!IsMoving())
                    return;

                _gameObject.transform.position += _velocity * deltaTime;
            }

            void Stop()
            {
                _velocity = Vector3::Zero;
            }

        public:
            PlayerMovement(GameObject& gameObject) : Component(gameObject) {}

            bool IsMoving() const { return _velocity.Length() > 0.01f; }
            bool HasInput() const { return _inputDirection.Length() > 0.0f; }

            void Update(float deltaTime) override
            {
                UpdateCameraRotation(deltaTime);
                UpdateDirection();
                UpdateVelocity(deltaTime);
                UpdatePosition(deltaTime);
            }
        };
    }
}
