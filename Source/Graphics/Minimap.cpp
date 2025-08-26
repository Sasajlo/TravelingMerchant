#include <Graphics/Minimap.hpp>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>
#include <Game/MapIcon.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <cmath>

using namespace TM::Graphics;
using namespace TM::Core;

void Minimap::CreateFramebuffer()
{
    // Create framebuffer
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Create render texture
    glGenTextures(1, &_renderTexture);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)_size.x, (int)_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _minimapTextureId = _renderTexture;
}

void Minimap::DestroyFramebuffer()
{
    if (_renderTexture)
    {
        glDeleteTextures(1, &_renderTexture);
        _renderTexture = 0;
    }
    if (_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    _minimapTextureId = 0;
}

glm::vec2 Minimap::WorldToMinimap(const Vector3& worldPos)
{
    // Convert world position to minimap coordinates (0-1 range)
    // Use X and Z coordinates for the minimap (Y is up/down in 3D world)
    float x = (worldPos.x - _worldBoundsMin.x) / (_worldBoundsMax.x - _worldBoundsMin.x);
    float z = (worldPos.z - _worldBoundsMin.y) / (_worldBoundsMax.y - _worldBoundsMin.y);
    
    // Flip the Z coordinate to fix the orientation
    z = 1.0f - z;
    
    return glm::vec2(x, z);
}

void Minimap::GenerateMinimapTexture()
{
    if (!_framebuffer) return;

    // Bind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glViewport(0, 0, (int)_size.x, (int)_size.y);

    // Clear with transparent background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up orthographic projection for minimap rendering
    glm::mat4 projection = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f);

    // Use the same shader as ImageRenderer for consistency
    _shader.Use();
    _shader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));
    _shader.BindUniform1i("imageTex", 0);

    // Separate ground objects, flagged objects, and other objects
    std::vector<TM::Game::MapIcon*> groundIcons;
    std::vector<TM::Game::MapIcon*> flaggedIcons;
    std::vector<TM::Game::MapIcon*> otherIcons;
    
    for (auto* mapIcon : _mapIcons)
    {
        if (!mapIcon || !mapIcon->IsActive() || !mapIcon->GetGameObject()->IsActive()) continue;
        
        // Check if this is a ground object (has Ground tag or uses ground texture)
        bool isGround = mapIcon->GetGameObject()->HasTag("Ground") || 
                       mapIcon->GetIconPath().find("ground.png") != std::string::npos;
        
        // Check if this is a flagged object (always on top)
        bool isFlagged = mapIcon->IsAlwaysOnTop();
        
        if (isGround)
        {
            groundIcons.push_back(mapIcon);
        }
        else if (isFlagged)
        {
            flaggedIcons.push_back(mapIcon);
        }
        else
        {
            otherIcons.push_back(mapIcon);
        }
    }
    
    // Sort only non-ground, non-flagged icons by Y coordinate (descending) so higher icons are rendered after lower ones
    std::sort(otherIcons.begin(), otherIcons.end(), [this](TM::Game::MapIcon* a, TM::Game::MapIcon* b) {
        if (!a || !b) return false;
        Vector3 posA = a->GetGameObject()->transform.GetWorldPosition();
        Vector3 posB = b->GetGameObject()->transform.GetWorldPosition();
        glm::vec2 minimapPosA = WorldToMinimap(posA);
        glm::vec2 minimapPosB = WorldToMinimap(posB);
        return minimapPosA.y > minimapPosB.y; // Sort by Y coordinate descending
    });
    
    // Sort flagged icons by Y coordinate as well (so they maintain proper layering among themselves)
    std::sort(flaggedIcons.begin(), flaggedIcons.end(), [this](TM::Game::MapIcon* a, TM::Game::MapIcon* b) {
        if (!a || !b) return false;
        Vector3 posA = a->GetGameObject()->transform.GetWorldPosition();
        Vector3 posB = b->GetGameObject()->transform.GetWorldPosition();
        glm::vec2 minimapPosA = WorldToMinimap(posA);
        glm::vec2 minimapPosB = WorldToMinimap(posB);
        return minimapPosA.y > minimapPosB.y; // Sort by Y coordinate descending
    });
    
    // Combine icons in rendering order: ground first, then sorted other icons, then flagged icons on top
    std::vector<TM::Game::MapIcon*> sortedIcons;
    sortedIcons.insert(sortedIcons.end(), groundIcons.begin(), groundIcons.end());
    sortedIcons.insert(sortedIcons.end(), otherIcons.begin(), otherIcons.end());
    sortedIcons.insert(sortedIcons.end(), flaggedIcons.begin(), flaggedIcons.end());

    // Render each map icon
    for (auto* mapIcon : sortedIcons)
    {
        if (!mapIcon || !mapIcon->IsActive() || !mapIcon->GetGameObject()->IsActive()) continue;

        Vector3 worldPos = mapIcon->GetGameObject()->transform.GetWorldPosition();
        glm::vec2 minimapPos = WorldToMinimap(worldPos);

        // Convert to minimap pixel coordinates
        float x = minimapPos.x * _size.x;
        float y = minimapPos.y * _size.y;
        float iconSize = 16.0f * mapIcon->GetScale() / _scale; // Base icon size

        // Calculate icon bounds with pivot offset
        float pivotOffsetX = iconSize * mapIcon->GetPivot().x;
        float pivotOffsetY = iconSize * mapIcon->GetPivot().y;
        
        float x0 = x - pivotOffsetX;
        float y0 = y - pivotOffsetY;
        float x1 = x0 + iconSize;
        float y1 = y0 + iconSize;

        // Convert to normalized coordinates (0-1 range) for culling
        float x0_norm = x0 / _size.x;
        float y0_norm = y0 / _size.y;
        float x1_norm = x1 / _size.x;
        float y1_norm = y1 / _size.y;

        // Viewport culling - render icons if any part is within the minimap bounds
        if (x1_norm < 0.0f || x0_norm > 1.0f || y1_norm < 0.0f || y0_norm > 1.0f)
        {
            continue; // Skip this icon if it's completely outside the minimap bounds
        }

        // Convert to normalized coordinates (0-1 range) for rendering
        x0 /= _size.x;
        y0 /= _size.y;
        x1 /= _size.x;
        y1 /= _size.y;

        // Calculate center point for rotation
        float centerX = (x0 + x1) * 0.5f;
        float centerY = (y0 + y1) * 0.5f;
        
        // Get rotation from map icon (convert degrees to radians for internal calculation)
        float rotation = glm::radians(mapIcon->GetRotation());
        
        // Apply rotation transformation to vertices
        auto rotatePoint = [centerX, centerY, rotation](float x, float y) -> std::pair<float, float> {
            float cosRot = cos(-rotation);
            float sinRot = sin(-rotation);
            float dx = x - centerX;
            float dy = y - centerY;
            float rotatedX = centerX + dx * cosRot - dy * sinRot;
            float rotatedY = centerY + dx * sinRot + dy * cosRot;
            return {rotatedX, rotatedY};
        };

        // Generate rotated vertices
        auto [x0_rot, y0_rot] = rotatePoint(x0, y0); // bottom-left
        auto [x1_rot, y1_rot] = rotatePoint(x1, y0); // bottom-right
        auto [x2_rot, y2_rot] = rotatePoint(x1, y1); // top-right
        auto [x3_rot, y3_rot] = rotatePoint(x0, y1); // top-left

        // UVs for map icons (not flipped to keep icons upright)
        float verts[] = {
            // x,  y,    u,   v
            x0_rot, y0_rot,     0.0f, 1.0f,  // bottom-left
            x1_rot, y1_rot,     1.0f, 1.0f,  // bottom-right
            x2_rot, y2_rot,     1.0f, 0.0f,  // top-right

            x0_rot, y0_rot,     0.0f, 1.0f,  // bottom-left (duplicate)
            x2_rot, y2_rot,     1.0f, 0.0f,  // top-right (duplicate)
            x3_rot, y3_rot,     0.0f, 0.0f   // top-left
        };

        _shader.BindUniform4f("tintColor", 
            mapIcon->GetColor().r, 
            mapIcon->GetColor().g, 
            mapIcon->GetColor().b, 
            mapIcon->GetColor().a);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mapIcon->GetTextureId());

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);

    // Unbind framebuffer and restore viewport
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto windowSize = Engine::GetWindowSize();
    glViewport(0, 0, windowSize.width, windowSize.height);
}

void Minimap::Awake()
{
    _gameObject.AddTag("UI");

    _shader.Load("Shaders/Minimap.vert", "Shaders/Minimap.frag");

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // layout(location=0): vec2 position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

    // layout(location=1): vec2 uv
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

    glBindVertexArray(0);

    // Create framebuffer for minimap texture generation
    CreateFramebuffer();
}

void Minimap::Start()
{
    // Subscribe to the scene when the component starts
    SubscribeToScene();
}

void Minimap::Render()
{
    if (!IsActive() || !_gameObject.IsActive()) return;
    if (_minimapTextureId == 0) return;

    // Update world bounds based on player position if player is set
    if (_player)
    {
        UpdateWorldBoundsFromPlayer();
    }

    // Generate minimap texture if map icons are dirty
    if (_mapIconsDirty)
    {
        GenerateMinimapTexture();
        _mapIconsDirty = false;
    }

    auto size = Engine::GetWindowSize();
    glm::mat4 projection = glm::ortho(0.0f, (float)size.width, (float)size.height, 0.0f);

    // Use world position instead of local position to include parent transformations
    Vector3 worldPos = _gameObject.transform.GetWorldPosition();
    float posX = worldPos.x;
    float posY = worldPos.y;

    float offsetX = _pivot.x * _size.x;
    float offsetY = _pivot.y * _size.y;

    float x0 = posX - offsetX;
    float y0 = posY - offsetY;
    float x1 = x0 + _size.x;
    float y1 = y0 + _size.y;

    // UVs flipped on Y to match stbi/OpenGL orientation
    float verts[] = {
        // x,  y,    u,   v
        x0, y0,     0.0f, 0.0f,  // bottom-left
        x1, y0,     1.0f, 0.0f,  // bottom-right
        x1, y1,     1.0f, 1.0f,  // top-right

        x0, y0,     0.0f, 0.0f,  // bottom-left (duplicate)
        x1, y1,     1.0f, 1.0f,  // top-right (duplicate)
        x0, y1,     0.0f, 1.0f   // top-left
    };

    _shader.Use();
    _shader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));
    _shader.BindUniform1i("imageTex", 0);
    _shader.BindUniform4f("tintColor", _tint.r, _tint.g, _tint.b, _tint.a);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _minimapTextureId);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Minimap::Destroy()
{
    // Unsubscribe from the scene when the component is destroyed
    UnsubscribeFromScene();
    
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    _shader.Destroy();
    DestroyFramebuffer();
}

void Minimap::SetActive(bool active)
{
    // Call the base class SetActive first
    Component::SetActive(active);

    // If becoming active, resubscribe to the scene
    if (active)
    {
        SubscribeToScene();
    }
}

void Minimap::SubscribeToScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->SubscribeMinimap(this);
    
    // Get all existing map icons from the scene and add them to this minimap
    const auto& mapIcons = currentScene->GetMapIcons();
    for (auto* mapIcon : mapIcons)
    {
        if (mapIcon && mapIcon->IsActive())
        {
            AddMapIcon(mapIcon);
        }
    }
}

void Minimap::UnsubscribeFromScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->UnsubscribeMinimap(this);
}

void Minimap::SetWorldBounds(const glm::vec2& min, const glm::vec2& max)
{
    _worldBoundsMin = min;
    _worldBoundsMax = max;
    _mapIconsDirty = true; // Regenerate minimap when bounds change
}

void Minimap::UpdateWorldBoundsFromPlayer()
{
    if (!_player) return;
    
    Vector3 playerPos = _player->transform.GetWorldPosition();
    float range = 10.0f * _scale; // Base range from player
    
    _worldBoundsMin = glm::vec2(playerPos.x - range, playerPos.z - range);
    _worldBoundsMax = glm::vec2(playerPos.x + range, playerPos.z + range);
    _mapIconsDirty = true; // Regenerate minimap when bounds change
}

void Minimap::AddMapIcon(TM::Game::MapIcon* mapIcon)
{
    if (mapIcon && std::find(_mapIcons.begin(), _mapIcons.end(), mapIcon) == _mapIcons.end())
    {
        _mapIcons.push_back(mapIcon);
        _mapIconsDirty = true;
    }
}

void Minimap::RemoveMapIcon(TM::Game::MapIcon* mapIcon)
{
    auto it = std::find(_mapIcons.begin(), _mapIcons.end(), mapIcon);
    if (it != _mapIcons.end())
    {
        _mapIcons.erase(it);
        _mapIconsDirty = true;
    }
}

void Minimap::RefreshMapIcons()
{
    _mapIconsDirty = true;
}

bool Minimap::IsMouseOver()
{
    auto screenPosition = _gameObject.transform.GetWorldPosition();
    auto mousePosition = Input::GetMousePosition();
    return (mousePosition.x >= screenPosition.x &&
        mousePosition.x <= screenPosition.x + _size.x &&
        mousePosition.y >= screenPosition.y &&
        mousePosition.y <= screenPosition.y + _size.y);
}
