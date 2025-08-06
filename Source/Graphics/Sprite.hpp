#pragma once

#include <Core/GameObject.hpp>

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

		public:
			Sprite(GameObject& gameObject) : Component(gameObject) {}
			~Sprite() = default;

			void Start() override;

			void Bind();

			void SetTexture(std::string texturePath) { _texturePath = texturePath; }

			unsigned int GetTextureId() const { return _textureId; }
		};

	}

}