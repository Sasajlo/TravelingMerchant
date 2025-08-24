#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Scene.hpp"

namespace TM
{
	namespace Core
	{
		const std::string DEFAULT_SCENE_NAME = "Main Scene";

		class SceneManager
		{
		private:
			SceneManager() = default;

			static SceneManager& Instance()
			{
				static SceneManager instance;
				return instance;
			}

			std::unordered_map<std::string, std::unique_ptr<Scene>> _scenes;
			std::vector<std::string> _sceneOrder; // Maintains order for index-based access
			Scene* _activeScene = nullptr; // Raw pointer for easy access, SceneManager owns the Scene

		public:
			~SceneManager() = default;

			// Scene management
			static void AddScene(const std::string& name);
			static void SetActiveScene(const std::string& name);
			static void SetActiveScene(int index);
			static Scene* GetActiveScene();
			
			// Scene information
			static std::string GetActiveSceneName();
			static int GetActiveSceneIndex();
			static std::string GetSceneName(int index);
			static int GetSceneIndex(const std::string& name);
			static int GetSceneCount();
			static std::vector<std::string> GetAllSceneNames();
			
			// Scene validation
			static bool SceneExists(const std::string& name);
			static bool SceneExists(int index);
			
			static void Destroy();
		};
	}
}