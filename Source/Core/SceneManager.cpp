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

Scene* SceneManager::GetActiveScene()
{
	if (!Instance()._activeScene) 
	{
		AddScene(DEFAULT_SCENE_NAME);
	}

	return Instance()._activeScene;
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
	Instance()._activeScene = nullptr;
}
