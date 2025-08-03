#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>

#include "Scene.hpp"

namespace TM
{
	namespace Core
	{
		const std::string DEFAULT_SCENE_NAME = "Main Scene";

		class SceneManager
		{
		public:
			~SceneManager() = default;

			static void AddScene(const std::string& name);
			static void SetActiveScene(const std::string& name);
			static Scene* GetActiveScene();
			static void Destroy();


		private:
			SceneManager() = default;

			static SceneManager& Instance()
			{
				static SceneManager instance;
				return instance;
			}

			std::unordered_map<std::string, std::unique_ptr<Scene>> _scenes;
			Scene* _activeScene = nullptr; // Raw pointer for easy access, SceneManager owns the Scene
		};
	}
}