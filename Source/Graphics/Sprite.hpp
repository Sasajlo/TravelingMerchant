#pragma once

#include <Core/GameObject.hpp>
#include <Core/Time.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/Color.hpp>

using namespace TM::Core;

namespace TM
{
	namespace Graphics
	{
		enum class AnimationDirection
		{
			Horizontal, // Frames are arranged in a row
			Vertical    // Frames are arranged in a column
		};

		class Sprite : public Component
		{
		private:
			unsigned int _VBO, _VAO, _EBO;
			unsigned int _textureId = 0;
			glm::vec2 _pivot = glm::vec2(0.5f, 0.5f); // Default pivot at center
			TM::Utils::Color _color = TM::Utils::Color::WHITE; // Default color is white

			// Animation properties (optional)
			bool _isAnimated = false;
			int _totalFrames = 1;
			int _currentFrame = 0;
			float _frameRate = 12.0f; // Frames per second
			float _frameTimer = 0.0f;
			AnimationDirection _direction = AnimationDirection::Horizontal;
			bool _isPlaying = true;
			bool _loop = true;

			// Sprite sheet properties
			int _columns = 1;
			int _rows = 1;
			int _animationOffset = 0; // Add this to track current animation offset (row or column)

			// Add a callback for texture changes
			std::function<void(unsigned int)> _onTextureChanged;

			void UpdateVertexData(); // Update vertex positions based on pivot
			void UpdateTextureCoordinates(); // Update texture coordinates for animation

		public:
			Sprite(GameObject& gameObject) : Component(gameObject) {}
			~Sprite() = default;

			void Start() override;
			void Update(float deltaTime) override;

			void Bind();

			// Basic sprite methods
			void SetTexture(std::string texturePath);
			void SetPivot(float x, float y);
			glm::vec2 GetPivot() { return _pivot; }

			// Animation setup methods
			void SetSpriteSheet(std::string texturePath, int columns, int rows);
			void SetFrameRate(float fps) { _frameRate = fps; }
			float GetFrameRate() const { return _frameRate; }

			void SetTotalFrames(int frames) { _totalFrames = frames; }
			int GetTotalFrames() const { return _totalFrames; }

			void SetCurrentFrame(int frame);
			int GetCurrentFrame() const { return _currentFrame; }

			void SetDirection(AnimationDirection direction) { _direction = direction; }
			AnimationDirection GetDirection() const { return _direction; }

			// Animation offset control (works for both directions)
			void SetAnimationOffset(int offset);
			int GetAnimationOffset() const { return _animationOffset; }

			// Animation control methods
			void Play() { _isPlaying = true; }
			void Pause() { _isPlaying = false; }
			void Stop() { _isPlaying = false; _currentFrame = 0; _frameTimer = 0.0f; }

			void SetLooping(bool loop) { _loop = loop; }
			bool IsLooping() const { return _loop; }

			bool IsPlaying() const { return _isPlaying; }
			bool IsAnimated() const { return _isAnimated; }

			// Color methods
			void SetColor(const TM::Utils::Color& color) { _color = color; }
			void SetColor(float r, float g, float b, float a = 1.0f) { _color = { r, g, b, a }; }
			const TM::Utils::Color& GetColor() const { return _color; }
			TM::Utils::Color& GetColor() { return _color; }

			unsigned int GetTextureId() const { return _textureId; }

			// Set texture change callback
			void SetTextureChangedCallback(std::function<void(unsigned int)> callback) { _onTextureChanged = callback; }
		};

	}
}