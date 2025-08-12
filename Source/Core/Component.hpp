#pragma once

#include <iostream>
#include <string>

namespace TM
{
    namespace Core
    {

        class Component
        {
            // Friend class to allow GameObject to access private members
            friend class GameObject;

        protected:
            GameObject& _gameObject;
            bool _isActive = true;

        public:
            Component(GameObject& gameObject) : _gameObject(gameObject) {}
            ~Component() = default;

            void SetActive(bool active) { _isActive = active; }
            bool IsActive() { return _isActive; }

            // Virtual methods that components can override
            virtual void Awake() {}
            virtual void Start() {}
            virtual void Update(float deltaTime) {}
            virtual void Render() {}
            virtual void Destroy() { }

            // Getters
            GameObject& GetGameObject() const { return _gameObject; }
        };
    }
}