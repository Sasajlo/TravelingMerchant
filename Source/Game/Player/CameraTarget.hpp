#pragma once

#include <iostream>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Graphics/Camera.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class CameraTarget : public Component
        {
        private:
			GameObject* _target = nullptr;

        public:
            CameraTarget(GameObject& gameObject) : Component(gameObject) {}

			void SetTarget(GameObject* target) { _target = target; }

            void Update(float deltaTime) override
            {
				if (!_target) return;
                Vector3 targetPosition = {
                    _target->transform.position.x,
                    _gameObject.transform.position.y,
                    _target->transform.position.z
                };

				_gameObject.transform.position = _gameObject.transform.position.Lerp(targetPosition, 2.5f * deltaTime);
            }
        };
    }
}