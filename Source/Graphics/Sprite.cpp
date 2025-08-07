#include <Graphics/Sprite.hpp>

// Include stb_image for texture loading
#define STB_IMAGE_IMPLEMENTATION
#include "../../Extern/stb_image.h"

using namespace TM::Graphics;
using namespace TM::Core;

unsigned int indices[] = {
	0, 1, 2, // first triangle
	2, 3, 0  // second triangle
};

// Function to load texture
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Pixel-perfect rendering settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // No wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // No wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);     // Pixel-perfect minification
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);     // Pixel-perfect magnification

        stbi_image_free(data);
        std::cout << "Texture loaded successfully: " << path << std::endl;
    }
    else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Sprite::Start()
{
    std::cout << "Sprite::Start() called for GameObject: " << _gameObject.GetName() << std::endl;

    // Load the texture
    _textureId = loadTexture(_texturePath.c_str());

    std::cout << "Texture ID: " << _textureId << " for " << _gameObject.GetName() << std::endl;

    // Generate OpenGL objects
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    // Update vertex data with current pivot
    UpdateVertexData();

    std::cout << "Sprite::Start() completed successfully" << std::endl;
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

    // Vertex data for a quad with texture coordinates (adjusted for pivot)
    float vertices[] = {
        // positions              // texture coords
        left,  bottom, 0.0f,      0.0f, 0.0f,  // bottom-left
        right, bottom, 0.0f,      1.0f, 0.0f,  // bottom-right
        right, top,    0.0f,      1.0f, 1.0f,  // top-right
        left,  top,    0.0f,      0.0f, 1.0f   // top-left
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
