#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
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

            // Component storage using type indices - only smart pointers, no dangling references
            std::unordered_map<std::type_index, std::unique_ptr<Component>> _asleepComponents;
            std::unordered_map<std::type_index, std::unique_ptr<Component>> _awakenComponents;
            std::unordered_map<std::type_index, std::unique_ptr<Component>> _activeComponents;

        public:
            ~GameObject() = default;
			Transform _transform; // Built-in Transform component


            static GameObject* Create(const std::string& name = "Game Object");
            
            // Static method to find GameObject by name in current scene
            static GameObject* Find(const std::string& name);

            const std::string& GetName() const { return _name; }

            Transform GetTransform() { return _transform; }

            // Component management
            template<typename T>
            T* AddComponent()
            {
                // Check if component already exists
                if (HasComponent<T>())
                {
                    std::cout << "Component of type " << typeid(T).name() << " already exists on GameObject: " << _name << std::endl;
                    return GetComponent<T>();
                }

                // Create new component with GameObject reference
                auto component = std::make_unique<T>(*this);
                T* componentPtr = component.get();
                
                // Store component in asleep collection using type index
                std::type_index typeIndex = std::type_index(typeid(T));
                _asleepComponents[typeIndex] = std::move(component);
                
                std::cout << "Added component " << typeid(T).name() << " to GameObject: " << _name << std::endl;
                return componentPtr;
            }

            template<typename T>
            T* GetComponent()
            {
                // Check in all component collections
                std::type_index typeIndex = std::type_index(typeid(T));
                
                // Check asleep components first
                auto asleepIt = _asleepComponents.find(typeIndex);
                if (asleepIt != _asleepComponents.end())
                {
                    return static_cast<T*>(asleepIt->second.get());
                }
                
                // Check awakened components
                auto awakenedIt = _awakenComponents.find(typeIndex);
                if (awakenedIt != _awakenComponents.end())
                {
                    return static_cast<T*>(awakenedIt->second.get());
                }
                
                // Check active components
                auto activeIt = _activeComponents.find(typeIndex);
                if (activeIt != _activeComponents.end())
                {
                    return static_cast<T*>(activeIt->second.get());
                }
                
                return nullptr;
            }

            template<typename T>
            bool HasComponent() const
            {
                std::type_index typeIndex = std::type_index(typeid(T));
                
                return _asleepComponents.find(typeIndex) != _asleepComponents.end() ||
                       _awakenComponents.find(typeIndex) != _awakenComponents.end() ||
                       _activeComponents.find(typeIndex) != _activeComponents.end();
            }

            template<typename T>
            void RemoveComponent()
            {
                std::type_index typeIndex = std::type_index(typeid(T));
                
                // Remove from all collections
                _asleepComponents.erase(typeIndex);
                _awakenComponents.erase(typeIndex);
                _activeComponents.erase(typeIndex);
                
                std::cout << "Removed component " << typeid(T).name() << " from GameObject: " << _name << std::endl;
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