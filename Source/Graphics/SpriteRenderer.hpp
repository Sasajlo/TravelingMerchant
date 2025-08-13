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

			bool _enableSwaying = false;
			float _swayAmount = 0.02f;
			float _swaySpeed = 1.0f;

		public:
			SpriteRenderer(GameObject& gameObject) : Component(gameObject) {}
			~SpriteRenderer() = default;

			void Awake() override;
			void Render() override;
			void Destroy() override;

			void EnableSwaying(bool enable) { _enableSwaying = enable; }
			void SetSwayAmount(float amount) { _swayAmount = amount; }
			void SetSwaySpeed(float speed) { _swaySpeed = speed; }
		};
	}
}