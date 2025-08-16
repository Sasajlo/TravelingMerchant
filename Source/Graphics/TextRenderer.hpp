#pragma once

#include <Core/Component.hpp>
#include <Graphics/Shader.hpp>
#include <Utils/Color.hpp>
#include <string>
#include <vector>

#include "../../Extern/stb_truetype.h"
#include <Graphics/Core.hpp> // add for glm

namespace TM
{
    namespace Graphics
    {
        class TextRenderer : public TM::Core::Component
        {
        private:
            Shader _shader;
            unsigned int _vao = 0, _vbo = 0;
            unsigned int _atlasTex = 0;
            int _atlasW = 1024, _atlasH = 1024;
            std::vector<stbtt_bakedchar> _cdata; // 96 glyphs [32..127)
            std::string _fontPath;
            float _fontPixelHeight = 32.0f;
            std::string _text = "Hello, world!";
            TM::Utils::Color _color = TM::Utils::Color::WHITE;
            glm::vec2 _pivot{ 0.0f, 0.0f }; // (0,0)=top-left, (1,1)=bottom-right

        public:
            TextRenderer(TM::Core::GameObject& go) : Component(go) {}
            ~TextRenderer() = default;

            void Awake() override;
            void Start() override;
            void Render() override;
            void Destroy() override;

            void SetActive(bool active) override;

            // Subscription management
            void SubscribeToScene();
            void UnsubscribeFromScene();

            void SetFont(const std::string& path, float pixelHeight);
            void SetText(const std::string& text) { _text = text; }
            void SetColor(TM::Utils::Color c) { _color = c; }
            void SetPivot(float x, float y); // add
        };

    }
}