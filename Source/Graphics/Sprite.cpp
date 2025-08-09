#include <Graphics/Sprite.hpp>

using namespace TM::Graphics;
using namespace TM::Core;

unsigned int indices[] = {
	0, 1, 2, // first triangle
	2, 3, 0  // second triangle
};

void Sprite::Start()
{
    // Load the texture using the new Texture class
    _textureId = Texture::Load(_texturePath);

    // Generate OpenGL objects
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    // Update vertex data with current pivot
    UpdateVertexData();
}

void Sprite::Bind()
{
    glBindVertexArray(_VAO);
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

void Sprite::SetPivot(float x, float y)
{
    _pivot = { glm::clamp(x, 0.0f, 1.0f), glm::clamp(y, 0.0f, 1.0f) };
    UpdateVertexData();
}

void Sprite::UpdateVertexData()
{
    // Calculate vertex positions based on pivot
    // Pivot (0,0) = bottom-left, (1,1) = top-right, (0.5,0.5) = center
    float left = -_pivot.x;
    float right = 1.0f - _pivot.x;
    float bottom = -_pivot.y;
    float top = 1.0f - _pivot.y;

    // Vertex data for a quad with texture coordinates (FLIPPED texture coords)
    float vertices[] = {
        // positions              // texture coords (flipped Y)
        left,  bottom, 0.0f,      0.0f, 1.0f,  // bottom-left
        right, bottom, 0.0f,      1.0f, 1.0f,  // bottom-right
        right, top,    0.0f,      1.0f, 0.0f,  // top-right
        left,  top,    0.0f,      0.0f, 0.0f   // top-left
    };

    glBindVertexArray(_VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
