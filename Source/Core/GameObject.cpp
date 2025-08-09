#include "GameObject.hpp"
#include "SceneManager.hpp"

using namespace TM::Core;

GameObject::GameObject(const std::string& name)
    : _name(name), _transform(*this)
{
}

GameObject* GameObject::Create(const std::string& name)
{
    Scene* activeScene = SceneManager::GetActiveScene();
    if (activeScene == nullptr)
    {
        SceneManager::AddScene(DEFAULT_SCENE_NAME);
        SceneManager::SetActiveScene(DEFAULT_SCENE_NAME);
    }

    // Create a unique_ptr for the GameObject
    // Since Scene is a friend class, we can create GameObjects directly
    auto gameObject = std::unique_ptr<GameObject>(new GameObject(name));
    GameObject* gameObjectPtr = gameObject.get();
    
    // Add to scene (Scene will take ownership)
    SceneManager::GetActiveScene()->AddGameObject(std::move(gameObject));
    
    // Return raw pointer (Scene owns the GameObject now)
    return gameObjectPtr;
}

GameObject* GameObject::Find(const std::string& name)
{
    Scene* activeScene = SceneManager::GetActiveScene();
    if (activeScene == nullptr)
    {
        std::cout << "No active scene found. Cannot find GameObject: " << name << std::endl;
        return nullptr;
    }
    
    return activeScene->FindGameObject(name);
}

// Tag system implementation
void GameObject::AddTag(const std::string& tag)
{
    _tags.insert(tag);
}

void GameObject::RemoveTag(const std::string& tag)
{
    _tags.erase(tag);
}

bool GameObject::HasTag(const std::string& tag) const
{
    return _tags.find(tag) != _tags.end();
}

std::vector<GameObject*> GameObject::FindByTag(const std::string& tag)
{
    Scene* activeScene = SceneManager::GetActiveScene();
    if (activeScene == nullptr)
    {
        std::cout << "No active scene found. Cannot find GameObjects by tag: " << tag << std::endl;
        return {};
    }
    
    return activeScene->FindGameObjectsByTag(tag);
}

void GameObject::Awake()
{
    // Process only asleep components (fast)
    if (!_asleepComponents.empty())
    {
        for (auto& component : _asleepComponents)
        {
            // Call Awake directly on the component (raw pointer)
            component.second->Awake();
            _awakenComponents[component.first] = component.second;
        }
        _asleepComponents.clear();
    }
}

void GameObject::Start()
{
    // Process only awakened components (fast)
    if (!_awakenComponents.empty())
    {
        for (auto& component : _awakenComponents)
        {
            // Call Start directly on the component (raw pointer)
            component.second->Start();
        }
        _awakenComponents.clear();
    }
}

void GameObject::Update(float deltaTime)
{
    // Process only active components (fast)
    for (const auto& component : _activeComponents)
    {
        component.second->Update(deltaTime);
    }
}

void GameObject::Render()
{
    // Render all active components
    for (const auto& component : _activeComponents)
    {
        component.second->Render();
    }
}

void GameObject::Destroy()
{
    // Destroy all components in all collections
    for (auto& component : _asleepComponents)
    {
        component.second->Destroy();
    }

    for (auto& component : _awakenComponents)
    {
        component.second->Destroy();
    }

    for (auto& component : _activeComponents)
    {
        component.second->Destroy();
    }
    
    _asleepComponents.clear();
    _awakenComponents.clear();
    _activeComponents.clear();
}