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
        class Billboard : public Component
        {
        public:
            Billboard(GameObject& gameObject) : Component(gameObject) {}

            void Update(float deltaTime) override
            {
                GameObject& cameraObject = Camera::GetMain()->GetGameObject();
                /*_gameObject._transform._rotation = {
                    cameraRotation.x,
					cameraRotation.y,
                    cameraRotation.z
                };*/
                
                glm::vec3 direction = {
                    cameraObject._transform._position.x - _gameObject._transform._position.x,
                    cameraObject._transform._position.y - _gameObject._transform._position.y,
                    cameraObject._transform._position.z - _gameObject._transform._position.z
				};

				direction = glm::normalize(direction);

                auto quaternion = glm::quat(1, 0, 0, 0);
                if (glm::length(direction) > 0.0001f)
                    quaternion = glm::quatLookAt(direction, glm::vec3(0, 1, 0));

                glm::vec3 newRotation = glm::eulerAngles(quaternion);

                _gameObject._transform._rotation = {
                    glm::degrees(newRotation.x),
                    glm::degrees(newRotation.y),
                    glm::degrees(newRotation.z)
                };
            }
        };
    }
}