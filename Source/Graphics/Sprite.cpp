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
    if (_textureId == 0)
        _textureId = Texture::Load("Assets/Textures/Default/square.png");

    // Generate OpenGL objects
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    // Update vertex data with current pivot
    UpdateVertexData();
    SetAnimationOffset(_animationOffset);
}

void Sprite::Update(float deltaTime)
{
    // Only update animation if this sprite is animated
    if (!_isAnimated || _totalFrames <= 1)
        return;

    // Update frame timer
    _animationTimer += deltaTime;
    _frameTimer += deltaTime;

    if (_isFinished)
        _isFinished = false;

    // Check if it's time to advance to the next frame
    float frameDuration = 1.0f / _frameRate;
    if (_frameTimer >= frameDuration)
    {
        _frameTimer -= frameDuration;
        _currentFrame++;

        // Handle looping or stopping
        if (_currentFrame >= _totalFrames)
        {
            if (_loop)
            {
                _currentFrame = 0;
            }
            else
            {
                _currentFrame = _totalFrames - 1;
                _frameTimer = 0.0f;
                _isPlaying = false;
                _isFinished = true;
            }
        }

        // Update texture coordinates for the new frame
        //UpdateTextureCoordinates();
    }
}

void Sprite::Bind()
{
    glBindVertexArray(_VAO);
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

void Sprite::SetTexture(std::string texturePath, TextureFilter filter)
{
    _textureId = Texture::Load(texturePath, filter);
    _isAnimated = false; // Reset to static sprite
    _columns = 1;
    _rows = 1;
    _totalFrames = 1;
    _currentFrame = 0;

    //UpdateTextureCoordinates();

    // Notify about texture change
    if (_onTextureChanged)
    {
        _onTextureChanged(_textureId);
    }
}

glm::vec4 Sprite::GetCurrentTextureCoordinates() const
{
    if (!_isAnimated || _totalFrames <= 1)
    {
        // For static sprites, return full texture coordinates
        return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // (left, bottom, right, top)
    }

    //return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    // Calculate which frame to show based on current frame and direction
    int frameX, frameY;

    if (_direction == AnimationDirection::Horizontal)
    {
        // Frames are arranged horizontally (in rows)
        frameX = _currentFrame % _columns;
        frameY = _animationOffset; // Use the animation offset as row

        /*if (_gameObject.GetName() == "Player")
        {
            std::cout << _animationOffset << std::endl;
        }*/
    }
    else
    {
        // Frames are arranged vertically (in columns)
        frameX = _animationOffset; // Use the animation offset as column
        frameY = _currentFrame % _rows;
    }

    // Calculate texture coordinates for the current frame
    float frameWidth = 1.0f / _columns;
    float frameHeight = 1.0f / _rows;

    float texLeft = frameX * frameWidth;
    float texRight = (frameX + 1) * frameWidth;
    float texBottom = frameY * frameHeight;
    float texTop = (frameY + 1) * frameHeight;

    // Return as (left, bottom, right, top) - note Y coordinates are flipped
    return glm::vec4(texLeft, texBottom, texRight, texTop);
}

void Sprite::SetSpriteSheet(std::string texturePath, int columns, int rows, TextureFilter filter)
{
    _textureId = Texture::Load(texturePath, filter);
    _isAnimated = true; // Enable animation
    _columns = columns;
    _rows = rows;
    _totalFrames = _direction == AnimationDirection::Horizontal ? columns : rows;
    _currentFrame = 0;
    _frameTimer = 0.0f;
    _animationTimer = 0.0f;
    SetAnimationOffset(_animationOffset);

    //UpdateTextureCoordinates();

    // Notify about texture change
    if (_onTextureChanged)
    {
        _onTextureChanged(_textureId);
    }
}

void Sprite::SetCurrentFrame(int frame)
{
    if (!_isAnimated)
        return;

    _currentFrame = glm::clamp(frame, 0, _totalFrames - 1);
    _frameTimer = 0.0f;
    //UpdateTextureCoordinates();
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
        left,  bottom, 0.0f,      0.0f, 0.0f,  // bottom-left
        right, bottom, 0.0f,      1.0f, 0.0f,  // bottom-right
        right, top,    0.0f,      1.0f, 1.0f,  // top-right
        left,  top,    0.0f,      0.0f, 1.0f   // top-left
    };

    glBindVertexArray(_VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

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

void Sprite::SetAnimationOffset(int offset)
{
    if (!_isAnimated)
        return;

    if (_direction == AnimationDirection::Horizontal)
    {
        // For horizontal animation, offset represents the row
        // Invert the offset so that offset 0 gives the first row (top)
        _animationOffset = glm::clamp(offset, 0, _rows - 1);
    }
    else
    {
        // For vertical animation, offset represents the column
        // Invert the offset so that offset 0 gives the first column (left)
        _animationOffset = glm::clamp(_columns - 1 - offset, 0, _columns - 1);
    }

    //_currentFrame = 0; // Reset to first frame of the new offset
    //_frameTimer = 0.0f;
    //UpdateTextureCoordinates();
}

void Sprite::UpdateTextureCoordinates()
{
    if (!_isAnimated || _totalFrames <= 1)
        return;

    // Calculate which frame to show based on current frame and direction
    int frameX, frameY;

    if (_direction == AnimationDirection::Horizontal)
    {
        // Frames are arranged horizontally (in rows)
        frameX = _currentFrame % _columns;
        frameY = _animationOffset; // Use the animation offset as row
    }
    else
    {
        // Frames are arranged vertically (in columns)
        frameX = _animationOffset; // Use the animation offset as column
        frameY = _currentFrame % _rows;
    }

    // Calculate texture coordinates for the current frame
    float frameWidth = 1.0f / _columns;
    float frameHeight = 1.0f / _rows;

    float texLeft = frameX * frameWidth;
    float texRight = (frameX + 1) * frameWidth;
    // Don't flip Y coordinates here since they're already flipped in the vertex setup
    float texBottom = frameY * frameHeight;
    float texTop = (frameY + 1) * frameHeight;

    // Calculate vertex positions based on pivot
    float left = -_pivot.x;
    float right = 1.0f - _pivot.x;
    float bottom = -_pivot.y;
    float top = 1.0f - _pivot.y;

    // Update texture coordinates in the vertex buffer
    // Note: We need to flip the Y coordinates to match the original vertex setup
    float vertices[] = {
        // positions              // texture coords (flipped Y to match original)
        left,  bottom, 0.0f,      texLeft,  1.0f - texBottom,  // bottom-left
        right, bottom, 0.0f,      texRight, 1.0f - texBottom,  // bottom-right
        right, top,    0.0f,      texRight, 1.0f - texTop,     // top-right
        left,  top,    0.0f,      texLeft,  1.0f - texTop      // top-left
    };

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindVertexArray(0);
}