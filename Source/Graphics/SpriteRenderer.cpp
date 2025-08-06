#include <Graphics/SpriteRenderer.hpp>

#include <iostream>

#include <Graphics/Camera.hpp>

using namespace TM::Core;
using namespace TM::Graphics;

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
	_shader.BindUniform1i("texture1", _sprite->GetTextureId());
}

void SpriteRenderer::Render()
{
	_sprite = _gameObject.GetComponent<Sprite>();
	if (_sprite == nullptr) {
		return;
	}

	// Ensure the shader and sprite are valid
	glm::mat4 model = _gameObject._transform.GetModelMatrix();
	glm::mat4 view = Camera::GetMain()->GetViewMatrix();
	glm::mat4 projection = Camera::GetMain()->GetProjectionMatrix();
	
	// Use the shader program
	_shader.Use();

	// Bind uniforms
	_shader.BindUniformMatrix4fv("model", glm::value_ptr(model));
	_shader.BindUniformMatrix4fv("view", glm::value_ptr(view));
	_shader.BindUniformMatrix4fv("projection", glm::value_ptr(projection));

	// Bind the texture
	_sprite->Bind();

	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::Destroy()
{
    _shader.Destroy();
	std::cout << "SpriteRenderer destroyed." << std::endl;
}
