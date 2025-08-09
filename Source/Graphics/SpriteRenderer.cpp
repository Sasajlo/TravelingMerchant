#include <Graphics/SpriteRenderer.hpp>

#include <iostream>

#include <Graphics/Camera.hpp>
#include <Game/Interactable.hpp>

using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Game;

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
	_shader.BindUniform1i("texture1", 0);
	_shader.BindUniform4f("tintColor", 1.0f, 1.0f, 1.0f, 1.0f);
	_shader.BindUniform1f("whiten", 0.0f);
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

	float whiten = 0.0f;
	if (_gameObject.HasComponent<Interactable>()) {
		auto* interactable = _gameObject.GetComponent<Interactable>();
		if (interactable && interactable->IsHovered()) {
			whiten = 0.2f;
		}
	}
	_shader.BindUniform1f("whiten", whiten);

	// Bind the texture
	_sprite->Bind();

	// Draw the sprite
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind VAO to prevent state pollution
	glBindVertexArray(0);
}

void SpriteRenderer::Destroy()
{
    _shader.Destroy();
}
