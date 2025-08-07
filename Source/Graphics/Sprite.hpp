#pragma once

#include <Core/GameObject.hpp>
#include <glad/glad.h>

using namespace TM::Core;

namespace TM
{
	namespace Graphics
	{
		class Sprite : public Component
		{
		private:
			unsigned int _VBO, _VAO, _EBO;
			unsigned int _textureId;
			std::string _texturePath = "Assets/Textures/Default/square.png"; // Default texture path
			glm::vec2 _pivot = glm::vec2(0.5f, 0.5f); // Default pivot at center

			void UpdateVertexData(); // Update vertex positions based on pivot

		public:
			Sprite(GameObject& gameObject) : Component(gameObject) {}
			~Sprite() = default;

			void Start() override;

			void Bind();

			void SetTexture(std::string texturePath) { _texturePath = texturePath; }
			void SetPivot(float x, float y);

			unsigned int GetTextureId() const { return _textureId; }
		};

	}

}