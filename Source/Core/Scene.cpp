#include "Scene.hpp"

#include <iostream>

using namespace TM::Core;

Scene::Scene(std::string name)
	: _name(name)
{
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	_asleepObjects[gameObject->GetName()] = std::move(gameObject);
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
	for (auto& gameObject : _activeObjects)
	{
		gameObject.second->Render();
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
