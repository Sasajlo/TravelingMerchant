#pragma once

#include <iostream>
#include <string>

namespace TM
{
    namespace Core
    {
        class GameObject;

        class Component
        {
        public:
            Component(GameObject& gameObject) : _gameObject(gameObject) {}
            ~Component() = default;

            // Virtual methods that components can override
            virtual void Awake() {}
            virtual void Start() {}
            virtual void Update(float deltaTime) {}
            virtual void Render() {}
            virtual void Destroy() {}

            // Getters
            GameObject& GetGameObject() const { return _gameObject; }

        protected:
            GameObject& _gameObject;

            // Friend class to allow GameObject to access private members
            friend class GameObject;
        };
    }
}