#pragma once

#include "GameObject.hpp"
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

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
			
			// Find GameObject by name across all lifecycle collections (including children)
			GameObject* FindGameObject(const std::string& name);
			
			// Find GameObjects by tag across all lifecycle collections (including children)
			std::vector<GameObject*> FindGameObjectsByTag(const std::string& tag);

			// Get all root GameObjects (those without parents)
			std::vector<GameObject*> GetRootGameObjects();

			// Get all active GameObjects (including children)
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

			// Helper methods for finding objects in hierarchy
			GameObject* FindGameObjectInHierarchy(const std::string& name, GameObject* root);
			void FindGameObjectsByTagInHierarchy(const std::string& tag, GameObject* root, std::vector<GameObject*>& results);
			void GetAllGameObjectsInHierarchy(GameObject* root, std::vector<GameObject*>& results);
			
			// Helper methods for rendering
			void CollectRenderableObjects(GameObject* root, std::vector<std::pair<GameObject*, float>>& renderableObjects, 
                                        const glm::mat4& V, const glm::mat4& P);
			void RenderUIElements(GameObject* root);
		};
	}
}