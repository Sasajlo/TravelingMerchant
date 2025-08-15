#include "Scene.hpp"

#include <iostream>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/TextRenderer.hpp> 
#include <Graphics/ImageRenderer.hpp>

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
	auto it = _activeObjects.find(name);
	if (it != _activeObjects.end())
	{
		it->second->Destroy(); // Call Destroy logic
		_activeObjects.erase(it);
		return;
	}
	it = _awakenObjects.find(name);
	if (it != _awakenObjects.end())
	{
		it->second->Destroy(); // Call Destroy logic
		_awakenObjects.erase(it);
		return;
	}
	it = _asleepObjects.find(name);
	if (it != _asleepObjects.end())
	{
		it->second->Destroy(); // Call Destroy logic
		_asleepObjects.erase(it);
		return;
	}
	std::cerr << "GameObject '" << name << "' not found in any collection." << std::endl;
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

	for (auto& gameObject : _activeObjects)
	{
		gameObject.second->Update(deltaTime);
	}
}

void Scene::Render()
{
	// Get camera position
	glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
	Camera* mainCamera = Camera::GetMain();
	if (mainCamera) {
		const Transform& cameraTransform = mainCamera->GetGameObject().GetTransform();
		cameraPos = glm::vec3(
			cameraTransform.GetPosition().x,
			cameraTransform.GetPosition().y,
			cameraTransform.GetPosition().z
		);
	}
    
    // Check active objects
    for (auto& gameObject : _activeObjects) {
        if (gameObject.second->HasTag("Ground")) {
            gameObject.second->Render();
        }
    }
    
	std::vector<std::pair<GameObject*, float>> renderableObjects;

	glm::mat4 V(1.0f), P(1.0f);
	if (mainCamera) {
		V = mainCamera->GetViewMatrix();
		P = mainCamera->GetProjectionMatrix();
	}

	// Collect all renderable objects from hierarchy
	for (auto& gameObject : _activeObjects) {
		CollectRenderableObjects(gameObject.second.get(), renderableObjects, V, P);
	}

	std::sort(renderableObjects.begin(), renderableObjects.end(),
		[](const std::pair<GameObject*, float>& a, const std::pair<GameObject*, float>& b) {
			return a.second > b.second; // top first, bottom last (in front)
		});

	// Render other objects
	for (auto& [gameObject, ndcY] : renderableObjects) {
		gameObject->Render();
	}

	glDisable(GL_CULL_FACE);
	// Render text and UI elements
	for (auto& it : _activeObjects) {
		RenderUIElements(it.second.get());
	}
	glEnable(GL_CULL_FACE);
}

void Scene::CollectRenderableObjects(GameObject* root, std::vector<std::pair<GameObject*, float>>& renderableObjects, 
                                   const glm::mat4& V, const glm::mat4& P)
{
	if (!root->IsActive()) return;

	// Skip ground objects (already rendered)
	if (root->HasTag("Ground") || root->HasTag("UI")) return;

	if (root->GetComponent<SpriteRenderer>() != nullptr) {
		const Transform& objTransform = root->GetTransform();
		glm::vec3 worldPos(
			objTransform.GetWorldPosition().x,
			objTransform.GetWorldPosition().y,
			objTransform.GetWorldPosition().z
		);

		// Project to clip space, then to NDC
		glm::vec4 clip = P * V * glm::vec4(worldPos, 1.0f);
		if (clip.w == 0.0f) return;

		float ndcY = clip.y / clip.w; // -1 bottom ... +1 top

		renderableObjects.emplace_back(root, ndcY);
	}

	// Process children
	for (GameObject* child : root->GetChildren())
	{
		CollectRenderableObjects(child, renderableObjects, V, P);
	}
}

void Scene::RenderUIElements(GameObject* root)
{
	if (!root->IsActive()) return;

	// Render text and image renderers
	if (auto* tr = root->GetComponent<TM::Graphics::ImageRenderer>())
	{
		if (tr->IsActive())
			tr->Render();
	} 
	
	if (auto* tr = root->GetComponent<TM::Graphics::TextRenderer>())
	{
		if (tr->IsActive())
			tr->Render();
	}

	// Process children
	for (GameObject* child : root->GetChildren())
	{
		RenderUIElements(child);
	}
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
}
