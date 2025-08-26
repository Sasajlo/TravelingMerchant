#pragma once

#include "GameObject.hpp"
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

// Forward declarations
namespace TM
{
	namespace Graphics
	{
		class SpriteRenderer;
		class ImageRenderer;
		class TextRenderer;
		class Minimap;
	}
	namespace Game
	{
		class MapIcon;
	}
}

namespace TM
{
	namespace Core
	{
		class Scene
		{
			friend class SceneManager;
			friend class TM::Graphics::SpriteRenderer;
			friend class TM::Graphics::ImageRenderer;
			friend class TM::Graphics::TextRenderer;
			friend class TM::Graphics::Minimap;
			friend class TM::Game::MapIcon;

		public:
			Scene(std::string);
			~Scene() = default;

			void AddGameObject(std::unique_ptr<GameObject> gameObject);
			void RemoveGameObject(const std::string& name);
			void RemoveGameObjectDeferred(const std::string& name);
			void ProcessDeferredDeletions();
			
			// Find GameObject by name across all lifecycle collections (including children)
			GameObject* FindGameObject(const std::string& name);
			
			// Find GameObjects by tag across all lifecycle collections (including children)
			std::vector<GameObject*> FindGameObjectsByTag(const std::string& tag);

			// Get all root GameObjects (those without parents)
			std::vector<GameObject*> GetRootGameObjects();

			// Get all active GameObjects (including children)
			std::vector<GameObject*> GetActiveGameObjects();

			// SpriteRenderer subscription methods
			void SubscribeSpriteRenderer(TM::Graphics::SpriteRenderer* renderer, unsigned int textureId);
			void UnsubscribeSpriteRenderer(TM::Graphics::SpriteRenderer* renderer, unsigned int textureId);
			void UpdateSpriteRendererTexture(TM::Graphics::SpriteRenderer* renderer, unsigned int oldTextureId, unsigned int newTextureId);
			
			// Ground sprite management
			void AddGroundSprite(TM::Graphics::SpriteRenderer* renderer);
			void RemoveGroundSprite(TM::Graphics::SpriteRenderer* renderer);

			// UI Renderer subscription methods
			void SubscribeImageRenderer(TM::Graphics::ImageRenderer* renderer);
			void UnsubscribeImageRenderer(TM::Graphics::ImageRenderer* renderer);
			void SubscribeTextRenderer(TM::Graphics::TextRenderer* renderer);
			void UnsubscribeTextRenderer(TM::Graphics::TextRenderer* renderer);

			// Minimap subscription methods
			void SubscribeMinimap(TM::Graphics::Minimap* minimap);
			void UnsubscribeMinimap(TM::Graphics::Minimap* minimap);

					// MapIcon subscription methods
		void SubscribeMapIcon(TM::Game::MapIcon* mapIcon);
		void UnsubscribeMapIcon(TM::Game::MapIcon* mapIcon);
		const std::vector<TM::Game::MapIcon*>& GetMapIcons() const { return _mapIcons; }

			// Cleanup methods
			void CleanupInvalidRenderers();

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

			// Add to private section:
			std::vector<std::string> _objectsToDelete;

			// SpriteRenderer subscription system - grouped by texture ID
			std::unordered_map<unsigned int, std::vector<TM::Graphics::SpriteRenderer*>> _spriteRenderersByTexture;
			
			// Separate collection for ground sprites (rendered separately)
			std::vector<TM::Graphics::SpriteRenderer*> _groundSprites;

			// UI Renderer subscription systems
			std::vector<TM::Graphics::ImageRenderer*> _imageRenderers;
			std::vector<TM::Graphics::TextRenderer*> _textRenderers;

			// Minimap and MapIcon subscription systems
			std::vector<TM::Graphics::Minimap*> _minimaps;
			std::vector<TM::Game::MapIcon*> _mapIcons;

			// Helper methods for finding objects in hierarchy
			GameObject* FindGameObjectInHierarchy(const std::string& name, GameObject* root);
			void FindGameObjectsByTagInHierarchy(const std::string& tag, GameObject* root, std::vector<GameObject*>& results);
			void GetAllGameObjectsInHierarchy(GameObject* root, std::vector<GameObject*>& results);
		};
	}
}