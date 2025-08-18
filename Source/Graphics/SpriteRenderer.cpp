#include <Graphics/SpriteRenderer.hpp>

#include <iostream> 

#include <Graphics/Camera.hpp>
#include <Game/Interactable.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Game;

// Initialize static members
unsigned int SpriteRenderer::_instanceVBO = 0;
bool SpriteRenderer::_instanceVBOInitialized = false;
Shader SpriteRenderer::_instancedShader;

// Add these new static members
unsigned int SpriteRenderer::_batchVAO = 0;
unsigned int SpriteRenderer::_batchVBO = 0;
unsigned int SpriteRenderer::_batchEBO = 0;
bool SpriteRenderer::_batchVAOInitialized = false;

void SpriteRenderer::Awake()
{
    _shader.Load("Shaders/Sprite.vert", "Shaders/Sprite.frag");

    _sprite = _gameObject.GetComponent<Sprite>();

    if (_sprite == nullptr) {
        std::cerr << "Error: SpriteRenderer requires a Sprite component on the same GameObject!" << std::endl;
        std::cerr << "GameObject: " << _gameObject.GetName() << " does not have a Sprite component." << std::endl;
        return;
    }

    _shader.Use();
    _shader.BindUniform1i("texture1", 0);
    _shader.BindUniform4f("tintColor", _sprite->GetColor().r, _sprite->GetColor().g, _sprite->GetColor().b, _sprite->GetColor().a);
    _shader.BindUniform1f("whiten", 0.0f);

    // Initialize swaying uniforms
    _shader.BindUniform1f("time", 0.0f);
    _shader.BindUniform1i("enableSwaying", _enableSwaying);
    _shader.BindUniform1f("swayAmount", _swayAmount);
    _shader.BindUniform1f("swaySpeed", _swaySpeed);
}

void SpriteRenderer::Start()
{
    // Get the sprite component and set up texture change callback
    Sprite* sprite = _gameObject.GetComponent<Sprite>();
    if (!sprite) return;
    _sprite = sprite;
    _currentTextureId = sprite->GetTextureId();

    // Set up callback to be notified when texture changes
    sprite->SetTextureChangedCallback([this](unsigned int newTextureId) {
        this->UpdateTextureSubscription(newTextureId);
    });

    // Subscribe to the scene when the component starts
    SubscribeToScene();
}

void SpriteRenderer::Render()
{
    _sprite = _gameObject.GetComponent<Sprite>();
    if (_sprite == nullptr) {
        return;
    }

    // Get current time for swaying animation
    float currentTime = Time::totalTime;

    glm::mat4 model = _gameObject.transform.GetModelMatrix();
    glm::mat4 view = Camera::GetMain()->GetViewMatrix();
    glm::mat4 projection = Camera::GetMain()->GetProjectionMatrix();

    _shader.Use();

    // Bind uniforms
    _shader.BindUniformMatrix4fv("model", glm::value_ptr(model));
    _shader.BindUniformMatrix4fv("view", glm::value_ptr(view));
    _shader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));

    // Bind swaying uniforms
    _shader.BindUniform1f("time", currentTime);
    _shader.BindUniform1i("enableSwaying", _enableSwaying);
    _shader.BindUniform1f("swayAmount", _swayAmount);
    _shader.BindUniform1f("swaySpeed", _swaySpeed);

    // Use the sprite's color for tinting
    _shader.BindUniform4f("tintColor", _sprite->GetColor().r, _sprite->GetColor().g, _sprite->GetColor().b, _sprite->GetColor().a);

    _shader.BindUniform1f("whiten", IsHovered() ? 0.2f : 0.0f);

    // Bind the texture
    _sprite->Bind();

    // Draw the sprite
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind VAO to prevent state pollution
    glBindVertexArray(0);
}

void SpriteRenderer::SubscribeToScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    // Get the sprite component to determine texture ID
    Sprite* sprite = _gameObject.GetComponent<Sprite>();
    if (sprite)
    {
        _currentTextureId = sprite->GetTextureId();
        currentScene->SubscribeSpriteRenderer(this, _currentTextureId);
    }
}

void SpriteRenderer::UnsubscribeFromScene()
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    // Unsubscribe using the stored texture ID
    if (_currentTextureId != 0)
    {
        currentScene->UnsubscribeSpriteRenderer(this, _currentTextureId);
        _currentTextureId = 0;
    }
}

void SpriteRenderer::UpdateTextureSubscription(unsigned int newTextureId)
{
    // Get the current scene
    Scene* currentScene = SceneManager::GetActiveScene();
    if (!currentScene) return;

    // Update the subscription if texture ID changed
    if (_currentTextureId != newTextureId)
    {
        currentScene->UpdateSpriteRendererTexture(this, _currentTextureId, newTextureId);
        _currentTextureId = newTextureId;
    }
}

void SpriteRenderer::RenderBatch(const std::vector<SpriteRenderer*>& renderers)
{
    if (renderers.empty()) {
        return;
    }

    if (!_instancedShader.IsLoaded())
        _instancedShader.Load("Shaders/InstancedSprite.vert", "Shaders/InstancedSprite.frag");

    // Get the first renderer to use its sprite data (vertices and texture)
    SpriteRenderer* firstRenderer = renderers[0];
    if (!firstRenderer || !firstRenderer->_sprite) {
        return;
    }

    // Check if all renderers use the same texture
    unsigned int textureId = firstRenderer->_sprite->GetTextureId();

    // Get current time for swaying animation
    float currentTime = Time::totalTime;

    // Get view and projection matrices (same for all sprites)
    glm::mat4 view = Camera::GetMain()->GetViewMatrix();
    glm::mat4 projection = Camera::GetMain()->GetProjectionMatrix();

    // Use the instanced shader
    _instancedShader.Use();

    // Bind texture uniform
    _instancedShader.BindUniform1i("texture1", 0);

    // Bind view and projection uniforms (same for all instances)
    _instancedShader.BindUniformMatrix4fv("view", glm::value_ptr(view));
    _instancedShader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));

    // Bind swaying uniforms from first renderer
    _instancedShader.BindUniform1f("time", currentTime);
    _instancedShader.BindUniform1i("enableSwaying", firstRenderer->_enableSwaying);
    _instancedShader.BindUniform1f("swayAmount", firstRenderer->_swayAmount);
    _instancedShader.BindUniform1f("swaySpeed", firstRenderer->_swaySpeed);

    // Use the first renderer's color for tinting
    _instancedShader.BindUniform4f("tintColor", firstRenderer->_sprite->GetColor().r, firstRenderer->_sprite->GetColor().g, firstRenderer->_sprite->GetColor().b, firstRenderer->_sprite->GetColor().a);

    // Bind the texture
    firstRenderer->_sprite->Bind();

    // Prepare model matrices, whiten values, and texture coordinates for all instances
    std::vector<glm::mat4> modelMatrices;
    std::vector<float> whitenValues;
    std::vector<glm::vec4> textureCoordinates;
    modelMatrices.reserve(renderers.size());
    whitenValues.reserve(renderers.size());
    textureCoordinates.reserve(renderers.size());

    for (const auto& renderer : renderers) {
        if (!renderer->IsActive() || !renderer->_gameObject.IsActive()) continue;
        if (renderer && renderer->_sprite) {
            // Use the transform's GetModelMatrix method instead of manual calculation
            modelMatrices.push_back(renderer->_gameObject.transform.GetModelMatrix());

            // Calculate whiten value for this specific renderer
            whitenValues.push_back(renderer->IsHovered() ? 0.2f : 0.0f);

            // Get texture coordinates for this specific sprite
            textureCoordinates.push_back(renderer->_sprite->GetCurrentTextureCoordinates());
        }
    }

    // Initialize instance VBO if not already done
    if (!_instanceVBOInitialized) {
        glGenBuffers(1, &_instanceVBO);
        _instanceVBOInitialized = true;
    }

    // Bind the instance VBO and upload model matrices
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_DYNAMIC_DRAW);

    // Set up instance attributes for model matrices
    // We need to bind the model matrix as 4 vec4 attributes (mat4 = 4 vec4s)
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(3 + i); // Start at attribute 3 (0,1,2 are used by vertex data)
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(3 + i, 1); // This makes it an instanced attribute
    }

    // Create and bind whiten VBO
    unsigned int whitenVBO;
    glGenBuffers(1, &whitenVBO);
    glBindBuffer(GL_ARRAY_BUFFER, whitenVBO);
    glBufferData(GL_ARRAY_BUFFER, whitenValues.size() * sizeof(float), whitenValues.data(), GL_DYNAMIC_DRAW);

    // Set up whiten attribute
    glEnableVertexAttribArray(7); // Use attribute 7 for whiten values
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
    glVertexAttribDivisor(7, 1); // This makes it an instanced attribute

    // Create and bind texture coordinates VBO
    unsigned int texCoordsVBO;
    glGenBuffers(1, &texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(glm::vec4), textureCoordinates.data(), GL_DYNAMIC_DRAW);

    // Set up texture coordinates attribute
    glEnableVertexAttribArray(8); // Use attribute 8 for texture coordinates
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glVertexAttribDivisor(8, 1); // This makes it an instanced attribute

    // Draw all instances
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, modelMatrices.size());

    // Clean up
    for (int i = 0; i < 4; i++) {
        glDisableVertexAttribArray(3 + i);
    }
    glDisableVertexAttribArray(7);
    glDisableVertexAttribArray(8);
    glDeleteBuffers(1, &whitenVBO);
    glDeleteBuffers(1, &texCoordsVBO);
    glBindVertexArray(0);
}

bool SpriteRenderer::IsHovered()
{
    if (_hovered) return true;
    if (!_gameObject.GetParent()) return false;
    if (auto parentRenderer = _gameObject.GetParent()->GetComponent<SpriteRenderer>())
    {
        return parentRenderer->IsHovered();
    }

    return false;
}

void SpriteRenderer::Destroy()
{
    _shader.Destroy();
    UnsubscribeFromScene();
}