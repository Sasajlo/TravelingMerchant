#include "Scene.hpp"

#include <iostream>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/TextRenderer.hpp> 
#include <Graphics/ImageRenderer.hpp>
#include <algorithm>

using namespace TM::Core;
using namespace TM::Graphics;

Scene::Scene(std::string name)
	: _name(name)
{
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	_asleepObjects[gameObject->GetName()] = std::move(gameObject);
}

void Scene::RemoveGameObject(const std::string& name)
{
	// Mark for deferred deletion instead of immediate deletion
	RemoveGameObjectDeferred(name);
}

void Scene::RemoveGameObjectDeferred(const std::string& name)
{
	// Add to deletion queue instead of immediately deleting
	_objectsToDelete.push_back(name);
}

void Scene::ProcessDeferredDeletions()
{
	for (const auto& name : _objectsToDelete)
	{
		auto it = _activeObjects.find(name);
		if (it != _activeObjects.end())
		{
			it->second->Destroy();
			_activeObjects.erase(it);
			continue;
		}

		it = _awakenObjects.find(name);
		if (it != _awakenObjects.end())
		{
			it->second->Destroy();
			_awakenObjects.erase(it);
			continue;
		}

		it = _asleepObjects.find(name);
		if (it != _asleepObjects.end())
		{
			it->second->Destroy();
			_asleepObjects.erase(it);
			continue;
		}

		std::cerr << "GameObject '" << name << "' not found in any collection." << std::endl;
	}

	_objectsToDelete.clear();
}


GameObject* Scene::FindGameObject(const std::string& name)
{
	// Search in asleep objects first
	auto asleepIt = _asleepObjects.find(name);
	if (asleepIt != _asleepObjects.end())
	{
		return asleepIt->second.get();
	}

	// Search in awakened objects
	auto awakenedIt = _awakenObjects.find(name);
	if (awakenedIt != _awakenObjects.end())
	{
		return awakenedIt->second.get();
	}

	// Search in active objects
	auto activeIt = _activeObjects.find(name);
	if (activeIt != _activeObjects.end())
	{
		return activeIt->second.get();
	}

	for (auto& gameObject : _asleepObjects)
	{
		auto result = FindGameObjectInHierarchy(name, gameObject.second.get());
		if (result) return result;
	}

	for (auto& gameObject : _awakenObjects)
	{
		auto result = FindGameObjectInHierarchy(name, gameObject.second.get());
		if (result) return result;
	}

	for (auto& gameObject : _activeObjects)
	{
		auto result = FindGameObjectInHierarchy(name, gameObject.second.get());
		if (result) return result;
	}

	return nullptr;
}

GameObject* Scene::FindGameObjectInHierarchy(const std::string& name, GameObject* root)
{
	if (!root) return nullptr;

	if (root->GetName() == name)
	{
		return root;
	}

	// Search in children
	for (GameObject* child : root->GetChildren())
	{
		GameObject* result = FindGameObjectInHierarchy(name, child);
		if (result) return result;
	}

	return nullptr;
}

std::vector<GameObject*> Scene::FindGameObjectsByTag(const std::string& tag)
{
	std::vector<GameObject*> foundObjects;

	// Search in active objects (root objects only)
	for (auto& gameObject : _activeObjects)
	{
		FindGameObjectsByTagInHierarchy(tag, gameObject.second.get(), foundObjects);
	}

	return foundObjects;
}

void Scene::FindGameObjectsByTagInHierarchy(const std::string& tag, GameObject* root, std::vector<GameObject*>& results)
{
	if (root->HasTag(tag))
	{
		results.push_back(root);
	}

	// Search in children
	for (GameObject* child : root->GetChildren())
	{
		FindGameObjectsByTagInHierarchy(tag, child, results);
	}
}

std::vector<GameObject*> Scene::GetRootGameObjects()
{
	std::vector<GameObject*> rootObjects;
	rootObjects.reserve(_activeObjects.size());

	for (const auto& [name, gameObject] : _activeObjects)
	{
		rootObjects.push_back(gameObject.get());
	}

	return rootObjects;
}

std::vector<GameObject*> Scene::GetActiveGameObjects()
{
	std::vector<GameObject*> allObjects;

	// Get all objects from root objects
	for (const auto& [name, gameObject] : _activeObjects)
	{
		GetAllGameObjectsInHierarchy(gameObject.get(), allObjects);
	}

	return allObjects;
}

void Scene::GetAllGameObjectsInHierarchy(GameObject* root, std::vector<GameObject*>& results)
{
	results.push_back(root);

	// Add all children
	for (GameObject* child : root->GetChildren())
	{
		GetAllGameObjectsInHierarchy(child, results);
	}
}

void Scene::SubscribeSpriteRenderer(SpriteRenderer* renderer, unsigned int textureId)
{
	if (!renderer) return;

	// Check if this is a ground sprite and add it to ground collection
	if (renderer->GetGameObject()->HasTag("Ground"))
	{
		AddGroundSprite(renderer);
		return; // Don't add to texture groups
	}

	_spriteRenderersByTexture[textureId].push_back(renderer);
}

void Scene::UnsubscribeSpriteRenderer(SpriteRenderer* renderer, unsigned int textureId)
{
	if (!renderer) return;

	// Check if this is a ground sprite and remove it from ground collection
	if (renderer->GetGameObject()->HasTag("Ground"))
	{
		RemoveGroundSprite(renderer);
		return; // Don't remove from texture groups
	}

	auto textureGroupIt = _spriteRenderersByTexture.find(textureId);
	if (textureGroupIt != _spriteRenderersByTexture.end())
	{
		auto& renderers = textureGroupIt->second;
		renderers.erase(
			std::remove(renderers.begin(), renderers.end(), renderer),
			renderers.end()
		);

		// Remove the texture group if it's empty
		if (renderers.empty())
		{
			_spriteRenderersByTexture.erase(textureGroupIt);
		}
	}
}

void Scene::UpdateSpriteRendererTexture(SpriteRenderer* renderer, unsigned int oldTextureId, unsigned int newTextureId)
{
	if (!renderer) return;

	// Ground sprites don't need texture updates since they're rendered separately
	if (renderer->GetGameObject()->HasTag("Ground")) return;

	// Unsubscribe from old texture group
	UnsubscribeSpriteRenderer(renderer, oldTextureId);

	// Subscribe to new texture group
	SubscribeSpriteRenderer(renderer, newTextureId);
}

void Scene::AddGroundSprite(SpriteRenderer* renderer)
{
	if (!renderer) return;

	// Check if already in the collection
	auto it = std::find(_groundSprites.begin(), _groundSprites.end(), renderer);
	if (it == _groundSprites.end())
	{
		_groundSprites.push_back(renderer);
	}
}

void Scene::RemoveGroundSprite(SpriteRenderer* renderer)
{
	if (!renderer) return;

	auto it = std::find(_groundSprites.begin(), _groundSprites.end(), renderer);
	if (it != _groundSprites.end())
	{
		_groundSprites.erase(it);
	}
}

void Scene::SubscribeImageRenderer(ImageRenderer* renderer)
{
	if (!renderer) return;
	
	// Check if already in the collection
	auto it = std::find(_imageRenderers.begin(), _imageRenderers.end(), renderer);
	if (it == _imageRenderers.end())
	{
		_imageRenderers.push_back(renderer);
	}
}

void Scene::UnsubscribeImageRenderer(ImageRenderer* renderer)
{
	if (!renderer) return;
	
	auto it = std::find(_imageRenderers.begin(), _imageRenderers.end(), renderer);
	if (it != _imageRenderers.end())
	{
		_imageRenderers.erase(it);
	}
}

void Scene::SubscribeTextRenderer(TextRenderer* renderer)
{
	if (!renderer) return;
	
	// Check if already in the collection
	auto it = std::find(_textRenderers.begin(), _textRenderers.end(), renderer);
	if (it == _textRenderers.end())
	{
		_textRenderers.push_back(renderer);
	}
}

void Scene::UnsubscribeTextRenderer(TextRenderer* renderer)
{
	if (!renderer) return;
	
	auto it = std::find(_textRenderers.begin(), _textRenderers.end(), renderer);
	if (it != _textRenderers.end())
	{
		_textRenderers.erase(it);
	}
}

void Scene::CleanupInvalidRenderers()
{
	// Clean up ground sprites
	_groundSprites.erase(
		std::remove_if(_groundSprites.begin(), _groundSprites.end(),
			[](SpriteRenderer* renderer) {
				return renderer == nullptr;
			}),
		_groundSprites.end()
	);
	
	// Clean up texture groups
	for (auto it = _spriteRenderersByTexture.begin(); it != _spriteRenderersByTexture.end();)
	{
		auto& renderers = it->second;
		renderers.erase(
			std::remove_if(renderers.begin(), renderers.end(),
				[](SpriteRenderer* renderer) {
					return renderer == nullptr;
				}),
			renderers.end()
		);
		
		// Remove empty texture groups
		if (renderers.empty())
		{
			it = _spriteRenderersByTexture.erase(it);
		}
		else
		{
			++it;
		}
	}
	
	// Clean up image renderers
	_imageRenderers.erase(
		std::remove_if(_imageRenderers.begin(), _imageRenderers.end(),
			[](ImageRenderer* renderer) {
				return renderer == nullptr;
			}),
		_imageRenderers.end()
	);
	
	// Clean up text renderers
	_textRenderers.erase(
		std::remove_if(_textRenderers.begin(), _textRenderers.end(),
			[](TextRenderer* renderer) {
				return renderer == nullptr;
			}),
		_textRenderers.end()
	);
}

void Scene::Awake()
{
	if (_asleepObjects.empty()) return;

	for (auto& gameObject : _asleepObjects)
	{
		gameObject.second->Awake();
		_awakenObjects[gameObject.first] = std::move(gameObject.second);
	}

	_asleepObjects.clear();
}

void Scene::Start()
{
	if (_awakenObjects.empty()) return;

	for (auto& gameObject : _awakenObjects)
	{
		gameObject.second->Start();
		_activeObjects[gameObject.first] = std::move(gameObject.second);
	}
	_awakenObjects.clear();
}


void Scene::Update(float deltaTime)
{
	if (_activeObjects.empty()) return;

	// Clear any pending deletions from previous frame
	_objectsToDelete.clear();

	// Update all active objects
	for (auto& gameObject : _activeObjects)
	{
		gameObject.second->Update(deltaTime);
	}

	// Process all deferred deletions after all updates are complete
	ProcessDeferredDeletions();
}

void Scene::Render()
{
	// Clean up any invalid renderers first
	CleanupInvalidRenderers();
	
	// Get camera position
	glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
	Camera* mainCamera = Camera::GetMain();
	if (mainCamera) {
		glm::vec3 cameraPos = mainCamera->GetGameObject()->GetTransform().position.ToVec3();
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// Render ground sprites separately (first layer)
	if (!_groundSprites.empty())
	{
		SpriteRenderer::RenderBatch(_groundSprites);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);

	// Render all other SpriteRenderers grouped by texture for efficient batch rendering
	for (const auto& [textureId, renderers] : _spriteRenderersByTexture)
	{
		if (!renderers.empty())
		{
			SpriteRenderer::RenderBatch(renderers);
		}
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// Render UI elements using cached collections
	for (ImageRenderer* renderer : _imageRenderers)
	{
		if (renderer && renderer->IsActive())
		{
			renderer->Render();
		}
	}
	
	for (TextRenderer* renderer : _textRenderers)
	{
		if (renderer && renderer->IsActive())
		{
			renderer->Render();
		}
	}
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
}

void Scene::Destroy()
{
	// Smart pointers handle cleanup automatically
	// Just call Destroy() on each GameObject for cleanup logic
	for (auto& gameObject : _asleepObjects)
	{
		gameObject.second->Destroy();
	}

	for (auto& gameObject : _awakenObjects)
	{
		gameObject.second->Destroy();
	}

	for (auto& gameObject : _activeObjects)
	{
		gameObject.second->Destroy();
	}

	// Clear all collections (smart pointers will automatically delete GameObjects)
	_asleepObjects.clear();
	_awakenObjects.clear();
	_activeObjects.clear();
	_spriteRenderersByTexture.clear();
	_groundSprites.clear();
	_imageRenderers.clear();
	_textRenderers.clear();
}