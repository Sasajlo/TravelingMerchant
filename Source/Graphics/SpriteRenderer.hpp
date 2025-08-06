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

		public:
			SpriteRenderer(GameObject& gameObject) : Component(gameObject) {}
			~SpriteRenderer() = default;

		void Awake() override;
		void Render() override;
		void Destroy() override;
		};
	}
}