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

void GameObject::Awake()
{   
    // Awake all asleep components and move them to awakened collection
    if (!_asleepComponents.empty())
    {
        for (auto& component : _asleepComponents)
        {
            component.second->Awake();
            _awakenComponents[component.first] = std::move(component.second);
        }
        _asleepComponents.clear();
    }
}

void GameObject::Start()
{    
    // Start all awakened components and move them to active collection
    if (!_awakenComponents.empty())
    {
        for (auto& component : _awakenComponents)
        {
            component.second->Start();
            _activeComponents[component.first] = std::move(component.second);
        }
        _awakenComponents.clear();
    }
}

void GameObject::Update(float deltaTime)
{
    // Ensure Awake and Start are called for newly added components before Update
	Awake();
	Start();

    // Update all active components
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
    std::cout << "GameObject destroyed: " << _name << std::endl;
    
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