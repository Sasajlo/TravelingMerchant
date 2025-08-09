#include "Scene.hpp"

#include <iostream>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/Camera.hpp>

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
	
	// GameObject not found
	return nullptr;
}

std::vector<GameObject*> Scene::FindGameObjectsByTag(const std::string& tag)
{
	std::vector<GameObject*> foundObjects;
	
	// Search in active objects
	for (auto& gameObject : _activeObjects)
	{
		if (gameObject.second->HasTag(tag))
		{
			foundObjects.push_back(gameObject.second.get());
		}
	}
	
	return foundObjects;
}

std::vector<GameObject*> Scene::GetActiveGameObjects()
{
	std::vector<GameObject*> activeObjects;
	activeObjects.reserve(_activeObjects.size());

	for (const auto& [name, gameObject] : _activeObjects)
	{
		activeObjects.push_back(gameObject.get());
	}

	return activeObjects;
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

	for (auto& gameObject : _activeObjects) {
		// Skip ground objects (already rendered)
		if (gameObject.second->HasTag("Ground")) continue;

		if (gameObject.second->GetComponent<SpriteRenderer>() != nullptr) {
			const Transform& objTransform = gameObject.second->GetTransform();
			glm::vec3 worldPos(
				objTransform.GetPosition().x,
				objTransform.GetPosition().y,
				objTransform.GetPosition().z
			);

			// Project to clip space, then to NDC
			glm::vec4 clip = P * V * glm::vec4(worldPos, 1.0f);
			if (clip.w == 0.0f) continue;

			float ndcY = clip.y / clip.w; // -1 bottom ... +1 top

			// Optional: use depth as tie-breaker if needed
			//float ndcZ = clip.z / clip.w;

			renderableObjects.emplace_back(gameObject.second.get(), ndcY);
		}
	}

	std::sort(renderableObjects.begin(), renderableObjects.end(),
		[](const std::pair<GameObject*, float>& a, const std::pair<GameObject*, float>& b) {
			return a.second > b.second; // top first, bottom last (in front)
		});

	// Render other objects
	for (auto& [gameObject, ndcY] : renderableObjects) {
		gameObject->Render();
	}
	
	// Sort by distance (farthest first)
	std::sort(renderableObjects.begin(), renderableObjects.end(), 
		[](const std::pair<GameObject*, float>& a, const std::pair<GameObject*, float>& b) {
			return a.second > b.second;
		});
	
	// Render other objects
	for (auto& [gameObject, distance] : renderableObjects) {
		gameObject->Render();
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
