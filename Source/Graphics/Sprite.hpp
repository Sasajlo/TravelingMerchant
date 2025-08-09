#pragma once

#include <Core/GameObject.hpp>
#include <Graphics/Texture.hpp>

using namespace TM::Core;

namespace TM
{
	namespace Graphics
	{
		class Sprite : public Component
		{
		private:
			unsigned int _VBO, _VAO, _EBO;
			unsigned int _textureId = 0;
			glm::vec2 _pivot = glm::vec2(0.5f, 0.5f); // Default pivot at center

			void UpdateVertexData(); // Update vertex positions based on pivot

		public:
			Sprite(GameObject& gameObject) : Component(gameObject) {}
			~Sprite() = default;

			void Start() override;

			void Bind();

			void SetTexture(std::string texturePath);
			void SetPivot(float x, float y);
			glm::vec2 GetPivot() { return _pivot; }

			unsigned int GetTextureId() const { return _textureId; }
		};

	}

}