#include <Graphics/Camera.hpp>

#include <Core/Engine.hpp>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Game/Player.hpp>

using namespace TM::Graphics;
using namespace TM::Core;
using namespace TM::Game;

Camera* Camera::_mainCamera = nullptr; // Initialize static member

Camera::Camera(GameObject& gameObject) : Component(gameObject)
{
	if (!_mainCamera) _mainCamera = this;
	_gameObject._transform._position.z = 10.0f; // Default position behind the camera
}

void Camera::Awake()
{
	std::cout << "Camera component awake." << std::endl;
}

void Camera::Start()
{
	std::cout << "Camera component started." << std::endl;
	Player* player = GameObject::Find("Player")->GetComponent<Player>();
	if (player)
	{
		std::cout << "Player component found in Camera Start." << std::endl;
		player->CustomMethod(); // Call custom method in Player
	}
	else
	{
		std::cout << "Player component not found in Camera Start." << std::endl;
	}
}

void Camera::Update(float deltaTime)
{
	if (!_followTarget) return;

	// Get target position
	const Transform& targetTransform = _followTarget->_transform;

	// Calculate camera position (target position + offset)
	Vector3 cameraPos = _followTarget->_transform._position + _followOffset;

	// Update camera transform
	Transform& cameraTransform = _gameObject._transform;
	cameraTransform.SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}

void Camera::SetBackgroundColor(Color color)
{
	_backgroundColor = color;
}

Color Camera::GetBackgroundColor()
{
	return _backgroundColor;
}

void Camera::SetMainCamera()
{
	_mainCamera = this;
}

glm::mat4 Camera::GetViewMatrix() const
{
	// Calculate camera position and target
	glm::vec3 cameraPos = glm::vec3(
		_gameObject._transform._position.x,
		_gameObject._transform._position.y,
		_gameObject._transform._position.z
	);

	// Calculate target position
	glm::vec3 targetPos;
	if (_lookAtTarget != nullptr) {
		// Look at the target
		const Transform& targetTransform = _lookAtTarget->GetTransform();
		targetPos = glm::vec3(
			targetTransform.GetPosition().x,
			targetTransform.GetPosition().y,
			targetTransform.GetPosition().z
		);
	}
	else {
		// Look in the direction the camera is facing
		glm::vec3 cameraRotation = glm::vec3(
			glm::radians(_gameObject._transform._rotation.x),
			glm::radians(_gameObject._transform._rotation.y),
			glm::radians(_gameObject._transform._rotation.z)
		);

		// Calculate forward direction
		glm::vec3 forward = glm::vec3(
			-sin(cameraRotation.y) * cos(cameraRotation.x),
			sin(cameraRotation.x),
			-cos(cameraRotation.y) * cos(cameraRotation.x)
		);

		targetPos = cameraPos + forward;
	}

	// Calculate up vector (assuming Y is up)
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Create view matrix using lookAt
	return glm::lookAt(cameraPos, targetPos, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	float aspectRation = (float)Engine::GetWindowSize().width / (float)Engine::GetWindowSize().height;
	return glm::perspective(glm::radians(_fov), aspectRation, _nearPlane, _farPlane);
}
