#pragma once

#include <Core/GameObject.hpp>
#include <Core/Window.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Sprite.hpp>

using namespace TM::Core;

namespace TM
{
	namespace Graphics
	{
		class SpriteRenderer : public Component
		{
		private:
			Shader _shader;
			Sprite* _sprite;
			unsigned int _currentTextureId = 0; // Track current texture ID for subscription

			bool _enableSwaying = false;
			float _swayAmount = 0.02f;
			float _swaySpeed = 1.0f;

			bool _hovered = false;

			// Static members for batch rendering
			static unsigned int _instanceVBO;
			static bool _instanceVBOInitialized;
			static Shader _instancedShader;

			// Add these new static members for batch VAO
			static unsigned int _batchVAO;
			static unsigned int _batchVBO;
			static unsigned int _batchEBO;
			static bool _batchVAOInitialized;

		public:
			SpriteRenderer(GameObject& gameObject) : Component(gameObject) {}
			~SpriteRenderer() = default;

			void Awake() override;
			void Start() override;
			void Render() override;
			void Destroy() override;

			void EnableSwaying(bool enable) { _enableSwaying = enable; }
			void SetSwayAmount(float amount) { _swayAmount = amount; }
			void SetSwaySpeed(float speed) { _swaySpeed = speed; }

			void SetHovered(bool hovered) { _hovered = hovered; }
			bool IsHovered();

			// Subscription management
			void SubscribeToScene();
			void UnsubscribeFromScene();
			void UpdateTextureSubscription(unsigned int newTextureId);

			bool IsAnimationFinished() const { return _sprite->IsFinished(); }

			static void InitializeBatchVAO();

			static void RenderBatch(const std::vector<SpriteRenderer*>& renderers);
		};
	}
}