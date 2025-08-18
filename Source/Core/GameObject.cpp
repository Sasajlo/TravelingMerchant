#include "GameObject.hpp"
#include "SceneManager.hpp"

using namespace TM::Core;

GameObject::GameObject(const std::string& name)
    : _name(name), transform(*this)
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

// Parent-child hierarchy implementation
void GameObject::SetParent(GameObject* parent)
{
    // Remove from current parent if any
    if (_parent != nullptr)
    {
        _parent->RemoveChild(this);
    }

    // Set new parent
    _parent = parent;

    // Add to new parent's children list
    if (_parent != nullptr)
    {
        _parent->AddChild(this);
    }
}

void GameObject::AddChild(GameObject* child)
{
    // Check if child is not already in the list
    if (std::find(_children.begin(), _children.end(), child) == _children.end())
    {
        _children.push_back(child);
    }
}

void GameObject::RemoveChild(GameObject* child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
    {
        _children.erase(it);
    }
}

GameObject* GameObject::CreateChild(const std::string& name)
{
    // Create a new GameObject directly (not through Create which adds to scene)
    auto child = std::unique_ptr<GameObject>(new GameObject(name));
    GameObject* childPtr = child.get();

    // Set this as its parent (this will add it to children list)
    childPtr->SetParent(this);

    // Take ownership of the child (parent manages the child's lifecycle)
    _childrenOwnership.push_back(std::move(child));

    // Add to asleep children collection (will be processed in next Awake cycle)
    _asleepChildren.push_back(childPtr);

    return childPtr;
}

void GameObject::RemoveComponents()
{
    if (_componentsToRemove.empty()) return;

    for (const auto& typeIndex : _componentsToRemove)
    {
        // Call Destroy on the component before removing
        auto it = _activeComponents.find(typeIndex);
        if (it != _activeComponents.end())
        {
            it->second->Destroy();
            _activeComponents.erase(it);
        }
    }

    _componentsToRemove.clear();
}

void GameObject::Awake()
{
    // Process only asleep components (fast)
    if (!_asleepComponents.empty())
    {
        auto asleepCopy = _asleepComponents;
        _asleepComponents.clear();

        for (auto& component : asleepCopy)
        {
            // Call Awake directly on the component (raw pointer)
            component.second->Awake();
            _awakenComponents[component.first] = component.second;
        }
    }

    // Process asleep children (fast)
    if (!_asleepChildren.empty())
    {
        auto asleepCopy = _asleepChildren;
        _asleepChildren.clear();
        for (GameObject* child : asleepCopy)
        {
            child->Awake();
            _awakenChildren.push_back(child);
        }
    }
}

void GameObject::Start()
{
    // Process only awakened components (fast)
    if (!_awakenComponents.empty())
    {
        auto awakenCopy = _awakenComponents;
        _awakenComponents.clear();
        for (auto& component : awakenCopy)
        {
            // Call Start directly on the component (raw pointer)
            component.second->Start();
        }
    }

    // Process awakened children (fast)
    if (!_awakenChildren.empty())
    {
        auto awakenCopy = _awakenChildren;
        _awakenChildren.clear();
        for (GameObject* child : awakenCopy)
        {
            child->Start();
        }
    }
}

void GameObject::Update(float deltaTime)
{
    if (!IsActive()) return;

    if (!_asleepComponents.empty() || !_asleepChildren.empty())
        Awake();

    if (!_awakenComponents.empty() || !_awakenChildren.empty())
        Start();

    // Process only active components (fast)
    for (const auto& component : _activeComponents)
    {
        if (!component.second) continue;
        if (component.second->_isActive)
            component.second->Update(deltaTime);
    }

    // Update all children
    for (GameObject* child : _children)
    {
        if (child->_isActive)
            child->Update(deltaTime);
    }

    RemoveComponents();
}

void GameObject::Render()
{
    if (!IsActive()) return;

    // Render all active components
    for (const auto& component : _activeComponents)
    {
        if (component.second->_isActive)
            component.second->Render();
    }

    // Render all children
    for (GameObject* child : _children)
    {
        if (child->_isActive)
            child->Render();
    }
}

void GameObject::Destroy()
{
    // Destroy all children first (including those in the main _children collection)
    for (GameObject* child : _children)
    {
        child->Destroy();
    }

    for (GameObject* child : _awakenChildren)
    {
        child->Destroy();
    }
    for (GameObject* child : _asleepChildren)
    {
        child->Destroy();
    }

    _awakenChildren.clear();
    _asleepChildren.clear();
    _children.clear();

    // Remove from parent
    if (_parent != nullptr)
    {
        _parent->RemoveChild(this);
        _parent = nullptr;
    }

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