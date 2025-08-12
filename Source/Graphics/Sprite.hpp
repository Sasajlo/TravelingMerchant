#pragma once

#include <Core/GameObject.hpp>
#include <Graphics/Texture.hpp>
#include <Utils/Color.hpp>

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
			TM::Utils::Color _color = TM::Utils::Color::WHITE; // Default color is white

			void UpdateVertexData(); // Update vertex positions based on pivot

		public:
			Sprite(GameObject& gameObject) : Component(gameObject) {}
			~Sprite() = default;

			void Start() override;

			void Bind();

			void SetTexture(std::string texturePath);
			void SetPivot(float x, float y);
			glm::vec2 GetPivot() { return _pivot; }

			// Color methods
			void SetColor(const TM::Utils::Color& color) { _color = color; }
			void SetColor(float r, float g, float b, float a = 1.0f) { _color = { r, g, b, a }; }
			const TM::Utils::Color& GetColor() const { return _color; }
			TM::Utils::Color& GetColor() { return _color; }

			unsigned int GetTextureId() const { return _textureId; }
		};

	}

}