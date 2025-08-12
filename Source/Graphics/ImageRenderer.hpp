#pragma once

#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/Utils.hpp>
#include <Utils/Vector3.hpp>
#include <Graphics/Core.hpp> // glm
#include <string>

namespace TM
{
    namespace Graphics
    {
        class ImageRenderer : public TM::Core::Component
        {
        private:
            Shader _shader;
            unsigned int _vao = 0, _vbo = 0;
            unsigned int _textureId = 0;
            glm::vec2 _size{ 100.0f, 100.0f };
            glm::vec2 _pivot{ 0.0f, 0.0f }; // (0,0)=top-left, (1,1)=bottom-right
            TM::Utils::Color _tint{ 1.0f, 1.0f, 1.0f, 1.0f };

            void UpdateSizeFromTexture();

        public:
            ImageRenderer(TM::Core::GameObject& go) : Component(go) {}
            ~ImageRenderer() = default;

            void Awake() override;
            void Render() override;
            void Destroy() override;

            void SetImage(const std::string& path);
            void SetSize(float w, float h) { _size = { w, h }; }
            Size GetSize() { return { (int) _size.x, (int) _size.y }; }
            void SetPivot(float x, float y) { _pivot = glm::clamp(glm::vec2{ x, y }, glm::vec2(0.0f), glm::vec2(1.0f)); }
            Vector3 GetPivot() { return Vector3(_pivot.x, _pivot.y, 0.0f); }
            
            // Color methods (matching Sprite interface for consistency)
            void SetColor(const TM::Utils::Color& color) { _tint = color; }
            void SetColor(float r, float g, float b, float a = 1.0f) { _tint = { r, g, b, a }; }
            void SetColor(int r, int g, int b, int a = 255) { _tint = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }; }
            const TM::Utils::Color& GetColor() const { return _tint; }
            TM::Utils::Color& GetColor() { return _tint; }

            bool IsMouseOver();
            
            // Legacy method (kept for backward compatibility)
            void SetTint(TM::Utils::Color c) { _tint = c; }
        };
    }
}