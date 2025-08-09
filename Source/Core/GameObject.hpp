#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "Component.hpp"
#include "Transform.hpp"

namespace TM
{
    namespace Core
    {
        class GameObject
        {
            friend class Scene; // Allow Scene to create GameObjects

        private:
            GameObject(const std::string& name = "Game Object");
            std::string _name;
			bool _isActive = true;

            // Tag system
            std::unordered_set<std::string> _tags;

            // Performance collections with raw pointers (no ownership)
            std::unordered_map<std::type_index, Component*> _asleepComponents;
            std::unordered_map<std::type_index, Component*> _awakenComponents;

            // Single ownership collection - also serves as active performance collection
            std::unordered_map<std::type_index, std::unique_ptr<Component>> _activeComponents;

        public:
            ~GameObject() = default;
			Transform _transform; // Built-in Transform component


            static GameObject* Create(const std::string& name = "Game Object");
            
            // Static method to find GameObject by name in current scene
            static GameObject* Find(const std::string& name);

            const std::string& GetName() const { return _name; }
			void SetName(const std::string& name) { _name = name; }

			const bool IsActive() const { return _isActive; }
			void SetActive(bool active) { _isActive = active; }

            Transform GetTransform() { return _transform; }

            // Tag system methods
            void AddTag(const std::string& tag);
            void RemoveTag(const std::string& tag);
            bool HasTag(const std::string& tag) const;
            const std::unordered_set<std::string>& GetTags() const { return _tags; }
            
            // Static method to find GameObjects by tag in current scene
            static std::vector<GameObject*> FindByTag(const std::string& tag);

            // Component management
            template<typename T>
            T* AddComponent()
            {
                if (HasComponent<T>())
                {
                    return GetComponent<T>();
                }

                auto component = std::make_unique<T>(*this);
                T* componentPtr = component.get();

                std::type_index typeIndex = std::type_index(typeid(T));

                // Only _activeComponents owns the component
                _activeComponents[typeIndex] = std::move(component);
                
                // Performance collections just reference the component
                _asleepComponents[typeIndex] = componentPtr;

                return componentPtr;
            }

            template<typename T>
            T* GetComponent()
            {
                // Always get from active collection - stable pointers
                std::type_index typeIndex = std::type_index(typeid(T));
                auto it = _activeComponents.find(typeIndex);
                return it != _activeComponents.end() ? static_cast<T*>(it->second.get()) : nullptr;
            }

            template<typename T>
            bool HasComponent() const
            {
                std::type_index typeIndex = std::type_index(typeid(T));
                return _activeComponents.find(typeIndex) != _activeComponents.end();
            }

            template<typename T>
            void RemoveComponent()
            {
                std::type_index typeIndex = std::type_index(typeid(T));

                // Remove from all collections
                _asleepComponents.erase(typeIndex);
                _awakenComponents.erase(typeIndex);
                _activeComponents.erase(typeIndex);

            }

            // Lifecycle methods
            void Awake();
            void Start();
            void Update(float deltaTime);
            void Render();
            void Destroy();
        };
    }
}