#pragma once

#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/Utils.hpp>
#include <Utils/Vector3.hpp>
#include <Graphics/Core.hpp> // glm
#include <vector>

using namespace TM::Core;

// Forward declaration
namespace TM::Game
{
    class MapIcon;
}

namespace TM
{
    namespace Graphics
    {
        class Minimap : public Component
        {
        private:
            Shader _shader;
            unsigned int _vao = 0, _vbo = 0;
            unsigned int _minimapTextureId = 0;
            unsigned int _framebuffer = 0;
            unsigned int _renderTexture = 0;
            
            glm::vec2 _size{ 180.0f, 180.0f };
            glm::vec2 _pivot{ 0.5f, 0.5f }; // Center pivot for minimap
            TM::Utils::Color _tint{ 1.0f, 1.0f, 1.0f, 1.0f };
            
            // World bounds for minimap (will be updated based on player position)
            glm::vec2 _worldBoundsMin{ -10.0f, -10.0f };
            glm::vec2 _worldBoundsMax{ 10.0f, 10.0f };
            
            // Minimap scale (higher values = zoom out, see more area)
            float _scale = 1.0f;
            
            // Player reference for centering
            GameObject* _player = nullptr;
            
            // Cached map icons
            std::vector<TM::Game::MapIcon*> _mapIcons;
            bool _mapIconsDirty = true;

            void CreateFramebuffer();
            void DestroyFramebuffer();
            void GenerateMinimapTexture();
            glm::vec2 WorldToMinimap(const Vector3& worldPos);

        public:
            Minimap(TM::Core::GameObject& go) : Component(go) {}
            ~Minimap() = default;

            void Awake() override;
            void Start() override;
            void Render() override;
            void Destroy() override;

            void SetActive(bool active) override;

            // Subscription management
            void SubscribeToScene();
            void UnsubscribeFromScene();

            // Minimap configuration
            void SetSize(float w, float h) { _size = { w, h }; }
            Size GetSize() { return { (int)_size.x, (int)_size.y }; }
            void SetPivot(float x, float y) { _pivot = glm::clamp(glm::vec2{ x, y }, glm::vec2(0.0f), glm::vec2(1.0f)); }
            Vector3 GetPivot() { return Vector3(_pivot.x, _pivot.y, 0.0f); }
            
            // World bounds
            void SetWorldBounds(const glm::vec2& min, const glm::vec2& max);
            void UpdateWorldBoundsFromPlayer();
            glm::vec2 GetWorldBoundsMin() const { return _worldBoundsMin; }
            glm::vec2 GetWorldBoundsMax() const { return _worldBoundsMax; }
            
            // Scale methods
            void SetScale(float scale) { _scale = scale; _mapIconsDirty = true; }
            float GetScale() const { return _scale; }
            
            // Player centering
            void SetPlayer(GameObject* player) { _player = player; }
            GameObject* GetPlayer() const { return _player; }
            
            // Color methods
            void SetColor(const TM::Utils::Color& color) { _tint = color; }
            void SetColor(float r, float g, float b, float a = 1.0f) { _tint = { r, g, b, a }; }
            void SetColor(int r, int g, int b, int a = 255) { _tint = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }; }
            const TM::Utils::Color& GetColor() const { return _tint; }
            TM::Utils::Color& GetColor() { return _tint; }

            // Map icon management
            void AddMapIcon(TM::Game::MapIcon* mapIcon);
            void RemoveMapIcon(TM::Game::MapIcon* mapIcon);
            void RefreshMapIcons();

            bool IsMouseOver();
        };
    }
}
