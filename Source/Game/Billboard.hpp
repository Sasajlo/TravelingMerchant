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
                _gameObject.transform.rotation = cameraObject.transform.rotation;
            }
        };
    }
}