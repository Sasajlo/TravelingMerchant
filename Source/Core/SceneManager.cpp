#include <Core/SceneManager.hpp>

using namespace TM::Core;

void SceneManager::AddScene(const std::string& name)
{
	if (Instance()._scenes.find(name) != Instance()._scenes.end())
	{
		std::cerr << "Scene with name '" << name << "' already exists!" << std::endl;
		return;
	}

	// Create a new scene and add it to the manager
	auto scene = std::make_unique<Scene>(name);
	Scene* scenePtr = scene.get();
	Instance()._scenes[name] = std::move(scene);
	Instance()._sceneOrder.push_back(name); // Add to order vector
	Instance()._activeScene = scenePtr; // Set the new scene as the active scene
}

void SceneManager::SetActiveScene(const std::string& name)
{
	if (Instance()._scenes.find(name) == Instance()._scenes.end())
	{
		std::cerr << "Scene with name '" << name << "' does not exist!" << std::endl;
		return;
	}
	Instance()._activeScene = Instance()._scenes[name].get();
}

void SceneManager::SetActiveScene(int index)
{
	if (index < 0 || index >= static_cast<int>(Instance()._sceneOrder.size()))
	{
		std::cerr << "Scene index " << index << " is out of range! Available scenes: 0-" 
				  << (Instance()._sceneOrder.size() - 1) << std::endl;
		return;
	}
	
	const std::string& sceneName = Instance()._sceneOrder[index];
	Instance()._activeScene = Instance()._scenes[sceneName].get();
}

Scene* SceneManager::GetActiveScene()
{
	if (!Instance()._activeScene) 
	{
		AddScene(DEFAULT_SCENE_NAME);
	}

	return Instance()._activeScene;
}

std::string SceneManager::GetActiveSceneName()
{
	if (!Instance()._activeScene)
	{
		GetActiveScene(); // This will create default scene if none exists
	}
	
	// Find the name of the active scene
	for (const auto& pair : Instance()._scenes)
	{
		if (pair.second.get() == Instance()._activeScene)
		{
			return pair.first;
		}
	}
	
	return DEFAULT_SCENE_NAME;
}

int SceneManager::GetActiveSceneIndex()
{
	std::string activeName = GetActiveSceneName();
	return GetSceneIndex(activeName);
}

std::string SceneManager::GetSceneName(int index)
{
	if (index < 0 || index >= static_cast<int>(Instance()._sceneOrder.size()))
	{
		std::cerr << "Scene index " << index << " is out of range! Available scenes: 0-" 
				  << (Instance()._sceneOrder.size() - 1) << std::endl;
		return "";
	}
	
	return Instance()._sceneOrder[index];
}

int SceneManager::GetSceneIndex(const std::string& name)
{
	for (int i = 0; i < static_cast<int>(Instance()._sceneOrder.size()); ++i)
	{
		if (Instance()._sceneOrder[i] == name)
		{
			return i;
		}
	}
	
	return -1; // Scene not found
}

int SceneManager::GetSceneCount()
{
	return static_cast<int>(Instance()._sceneOrder.size());
}

std::vector<std::string> SceneManager::GetAllSceneNames()
{
	return Instance()._sceneOrder;
}

bool SceneManager::SceneExists(const std::string& name)
{
	return Instance()._scenes.find(name) != Instance()._scenes.end();
}

bool SceneManager::SceneExists(int index)
{
	return index >= 0 && index < static_cast<int>(Instance()._sceneOrder.size());
}

void SceneManager::Destroy()
{
	// Call Destroy() on all scenes for cleanup logic
	for (auto& scene : Instance()._scenes)
	{
		scene.second->Destroy();
	}
	
	// Smart pointers handle automatic cleanup
	Instance()._scenes.clear();
	Instance()._sceneOrder.clear();
	Instance()._activeScene = nullptr;
}
