#pragma once

#include <Core/GameObject.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/Utils.hpp>
#include <string>

using namespace TM::Core;
using namespace TM::Graphics;

namespace TM
{
    namespace Game
    {
        class MapIcon : public Component
        {
        private:
            std::string _iconPath;
            unsigned int _textureId = 0;
            TM::Utils::Color _tint{ 1.0f, 1.0f, 1.0f, 1.0f };
            float _scale = 1.0f;
            glm::vec2 _pivot{ 0.5f, 0.5f }; // Center pivot by default
            bool _alwaysOnTop = false; // Flag for icons that should always render on top
            float _rotation = 0.0f; // Rotation in radians

        public:
            MapIcon(GameObject& gameObject) : Component(gameObject) {}
            ~MapIcon() = default;

            void Awake() override;
            void Start() override;
            void Destroy() override;

            // Subscription management
            void SubscribeToScene();
            void UnsubscribeFromScene();

            // Icon management
            void SetIcon(const std::string& path, TextureFilter filter = TextureFilter::LINEAR);
            void SetColor(const TM::Utils::Color& color) { _tint = color; }
            void SetColor(float r, float g, float b, float a = 1.0f) { _tint = { r, g, b, a }; }
            void SetScale(float scale) { _scale = scale; }
            void SetPivot(float x, float y) { _pivot = { x, y }; }
            void SetPivot(const glm::vec2& pivot) { _pivot = pivot; }

            // Flag management
            void SetAlwaysOnTop(bool alwaysOnTop) { _alwaysOnTop = alwaysOnTop; }
            bool IsAlwaysOnTop() const { return _alwaysOnTop; }

            // Rotation management
            void SetRotation(float rotationDegrees) { _rotation = glm::radians(rotationDegrees); }
            float GetRotation() const { return glm::degrees(_rotation); }

            // Getters
            const std::string& GetIconPath() const { return _iconPath; }
            unsigned int GetTextureId() const { return _textureId; }
            const TM::Utils::Color& GetColor() const { return _tint; }
            float GetScale() const { return _gameObject.transform.GetScale().x; }
            const glm::vec2& GetPivot() const { return _pivot; }
        };
    }
}
