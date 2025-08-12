#include <Graphics/TextRenderer.hpp>
#include <Core/Engine.hpp>
#include <Utils/Color.hpp>

#include <vector>
#include <fstream>
#include <iostream>

#define STB_TRUETYPE_IMPLEMENTATION 
#include "../../Extern/stb_truetype.h"

using namespace TM::Graphics;
using namespace TM::Core;
using namespace TM::Utils;

static std::vector<unsigned char> ReadFileBinary(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        std::cerr << "Failed to open font: " << path << std::endl;
        return {};
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

void TextRenderer::SetPivot(float x, float y)
{
    _pivot = glm::clamp(glm::vec2(x, y), glm::vec2(0.0f), glm::vec2(1.0f));
}

void TextRenderer::SetFont(const std::string& path, float pixelHeight)
{
    _fontPath = path;
    _fontPixelHeight = pixelHeight;

    auto data = ReadFileBinary(path);
    if (data.empty()) return;

    _atlasW = 1024; _atlasH = 1024;
    std::vector<unsigned char> bitmap(_atlasW * _atlasH, 0);

    _cdata.resize(96); // ASCII 32..127
    int baked = stbtt_BakeFontBitmap(data.data(), 0, pixelHeight,
        bitmap.data(), _atlasW, _atlasH,
        32, 96, _cdata.data());
    if (baked <= 0) {
        std::cerr << "stbtt_BakeFontBitmap failed for " << path << std::endl;
        return;
    }

    if (_atlasTex == 0) glGenTextures(1, &_atlasTex);
    glBindTexture(GL_TEXTURE_2D, _atlasTex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _atlasW, _atlasH, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Swizzle: sample alpha from red, rgb=1
    GLint swizzleMask[] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
}

void TextRenderer::Awake()
{
    _gameObject.AddTag("UI");

    _shader.Load("Shaders/TextUI.vert", "Shaders/TextUI.frag");

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

    if (_fontPath.empty()) {
        SetFont("Assets/Fonts/Minecraft.ttf", _fontPixelHeight);
    }
}

void TextRenderer::Render()
{
    if (_atlasTex == 0 || _text.empty()) return;

    auto size = Engine::GetWindowSize();
    glm::mat4 projection = glm::ortho(0.0f, (float)size.width, (float)size.height, 0.0f);

    _shader.Use();
    _shader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));
    _shader.BindUniform1i("fontAtlas", 0);
    _shader.BindUniform4f("textColor", _color.r, _color.g, _color.b, _color.a);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _atlasTex);

    std::vector<float> verts;
    verts.reserve(_text.size() * 6 * 4);

    // First pass: compute bounds in local coords (start at 0,0)
    float xLocal = 0.0f, yLocal = 0.0f;
    float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
    stbtt_aligned_quad q;
    for (char c : _text) {
        if (c < 32 || c >= 128) continue;
        stbtt_GetBakedQuad(_cdata.data(), _atlasW, _atlasH, c - 32, &xLocal, &yLocal, &q, 1);
        minX = std::min(minX, std::min(q.x0, q.x1));
        minY = std::min(minY, std::min(q.y0, q.y1));
        maxX = std::max(maxX, std::max(q.x0, q.x1));
        maxY = std::max(maxY, std::max(q.y0, q.y1));
    }
    if (minX == FLT_MAX) return; // nothing renderable

    float offsetX = minX + _pivot.x * (maxX - minX);
    float offsetY = minY + _pivot.y * (maxY - minY);

    // Second pass: build vertices with pivot offset, then translate to object position
    Vector3 worldPos = _gameObject.transform.GetWorldPosition();
    float posX = worldPos.x;
    float posY = worldPos.y;
    xLocal = 0.0f; yLocal = 0.0f;

    for (char c : _text) {
        if (c < 32 || c >= 128) continue;
        stbtt_GetBakedQuad(_cdata.data(), _atlasW, _atlasH, c - 32, &xLocal, &yLocal, &q, 1);

        float x0 = q.x0 - offsetX + posX;
        float y0 = q.y0 - offsetY + posY;
        float x1 = q.x1 - offsetX + posX;
        float y1 = q.y1 - offsetY + posY;

        // tri1
        verts.push_back(x0); verts.push_back(y0); verts.push_back(q.s0); verts.push_back(q.t0);
        verts.push_back(x1); verts.push_back(y0); verts.push_back(q.s1); verts.push_back(q.t0);
        verts.push_back(x1); verts.push_back(y1); verts.push_back(q.s1); verts.push_back(q.t1);
        // tri2
        verts.push_back(x0); verts.push_back(y0); verts.push_back(q.s0); verts.push_back(q.t0);
        verts.push_back(x1); verts.push_back(y1); verts.push_back(q.s1); verts.push_back(q.t1);
        verts.push_back(x0); verts.push_back(y1); verts.push_back(q.s0); verts.push_back(q.t1);
    }

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(verts.size() / 4));
    glBindVertexArray(0);
}

void TextRenderer::Destroy()
{
    if (_atlasTex) glDeleteTextures(1, &_atlasTex);
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    _shader.Destroy();
}