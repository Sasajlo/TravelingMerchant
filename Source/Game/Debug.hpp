#pragma once

#include <iostream>
#include <string>

#include <Core/Engine.hpp>
#include <Core/GameObject.hpp>
#include <Graphics/TextRenderer.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class Debug : public Component
        {
        private:
            TextRenderer* _fpsText = nullptr;
            float _timePassed = 0;
            const float UPDATE_TIME = 0.5f;

        public:
            Debug(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
                _fpsText = GameObject::Find("FPS Text")->GetComponent<TextRenderer>();
            }

            void Update(float deltaTime) override
            {
                _timePassed += deltaTime;
                if (_timePassed < UPDATE_TIME) return;

                std::string text = "FPS: " + std::to_string(Engine::GetFPS());
                _fpsText->SetText(text);

                _timePassed = 0;
            }
        };
    }
}