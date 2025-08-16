#include <Graphics/ImageRenderer.hpp>
#include <Core/Engine.hpp>
#include <Core/SceneManager.hpp>

using namespace TM::Graphics;
using namespace TM::Core;

void ImageRenderer::UpdateSizeFromTexture()
{
    if (_textureId == 0) return;
    glBindTexture(GL_TEXTURE_2D, _textureId);
    int w = 0, h = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    if (w > 0 && h > 0) _size = glm::vec2((float)w, (float)h);
}

void ImageRenderer::SetImage(const std::string& path)
{
    _textureId = Texture::Load(path);
    UpdateSizeFromTexture();
}

bool ImageRenderer::IsMouseOver()
{
    auto screenPosition = _gameObject.transform.GetWorldPosition();
    auto mousePosition = Input::GetMousePosition();
    return (mousePosition.x >= screenPosition.x &&
        mousePosition.x <= screenPosition.x + _size.x &&
        mousePosition.y >= screenPosition.y &&
        mousePosition.y <= screenPosition.y + _size.y);
}

void ImageRenderer::Awake()
{
    if (_gameObject.GetName() == "Slot") std::cout << "LOL" << std::endl;

    _gameObject.AddTag("UI");

    _shader.Load("Shaders/ImageUI.vert", "Shaders/ImageUI.frag");

    // Default image if none set
    if (_textureId == 0) {
        SetImage("Assets/Textures/Default/square.png");
    }

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
}

void ImageRenderer::Start()
{
    // Subscribe to the scene when the component starts
    SubscribeToScene();
}

void ImageRenderer::Render()
{
    if (!IsActive() || !_gameObject.IsActive()) return;
    if (_textureId == 0) return;

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

    // UVs flipped on Y to match stbi/OpenGL orientation (like Sprite)
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
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // Back to triangles with 6 vertices
    glBindVertexArray(0);
}

void ImageRenderer::Destroy()
{
    // Unsubscribe from the scene when the component is destroyed
    UnsubscribeFromScene();
    
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    _shader.Destroy();
}

void ImageRenderer::SetActive(bool active)
{
    // Call the base class SetActive first
    Component::SetActive(active);

    // If becoming active, resubscribe to the scene
    if (active)
    {
        SubscribeToScene();
    }
}

void ImageRenderer::SubscribeToScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->SubscribeImageRenderer(this);
}

void ImageRenderer::UnsubscribeFromScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    currentScene->UnsubscribeImageRenderer(this);
}