#pragma once

#include "GameObject.hpp"
#include <memory>
#include <unordered_map>

namespace TM
{
	namespace Core
	{
		class Scene
		{
			friend class SceneManager;

		public:
			Scene(std::string);
			~Scene() = default;

			void AddGameObject(std::unique_ptr<GameObject> gameObject);
			void RemoveGameObject(const std::string& name);
			
			// Find GameObject by name across all lifecycle collections
			GameObject* FindGameObject(const std::string& name);
			
			// Find GameObjects by tag across all lifecycle collections
			std::vector<GameObject*> FindGameObjectsByTag(const std::string& tag);

			std::vector<GameObject*> GetActiveGameObjects();

			void Awake();
			void Start();
			void Update(float deltaTime);
			void Render();
			void Destroy();

		private:
			std::string _name;
			std::unordered_map<std::string, std::unique_ptr<GameObject>> _asleepObjects;
			std::unordered_map<std::string, std::unique_ptr<GameObject>> _awakenObjects;
			std::unordered_map<std::string, std::unique_ptr<GameObject>> _activeObjects;
		};
	}
}