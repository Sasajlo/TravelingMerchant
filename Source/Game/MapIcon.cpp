#include <Game/MapIcon.hpp>
#include <Core/SceneManager.hpp>
#include <iostream>

using namespace TM::Game;
using namespace TM::Core;

void MapIcon::Awake()
{
    _gameObject.AddTag("MapIcon");
}

void MapIcon::Start()
{
    // Subscribe to the scene when the component starts
    SubscribeToScene();
}

void MapIcon::Destroy()
{
    // Unsubscribe from the scene when the component is destroyed
    UnsubscribeFromScene();
}

void MapIcon::SubscribeToScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->SubscribeMapIcon(this);
}

void MapIcon::UnsubscribeFromScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->UnsubscribeMapIcon(this);
}

void MapIcon::SetIcon(const std::string& path, TextureFilter filter)
{
    _iconPath = path;
    _textureId = Texture::Load(path, filter);
}
