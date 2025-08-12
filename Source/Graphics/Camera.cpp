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
	_gameObject.transform.position.z = 10.0f; // Default position behind the camera
}

void Camera::Awake()
{
	
}

void Camera::Start()
{
	
}

void Camera::Update(float deltaTime)
{
	if (_followTarget)
	{
		// Get target position
		const Transform& targetTransform = _followTarget->transform;

		// Calculate camera position (target position + offset)
		Vector3 cameraPos = _followTarget->transform.position + _followOffset;

		// Update camera transform
		_gameObject.transform.SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	}

	if (_lookAtTarget)
	{
		const Transform& targetTransform = _lookAtTarget->transform;
		// Calculate rotation to face target
		Vector3 dir = targetTransform.position - _gameObject.transform.position;
		dir = dir.Normalized();

		auto quaternion = glm::quatLookAt(
			glm::vec3(dir.x, dir.y, dir.z), // Direction vector
			glm::vec3(0.0f, 1.0f, 0.0f)     // Up vector
		);

		glm::vec3 eulerRadians = glm::eulerAngles(quaternion);

		// Convert to degrees for your transform
		_gameObject.transform.rotation = { glm::degrees(eulerRadians.x), glm::degrees(eulerRadians.y), glm::degrees(eulerRadians.z) };
	}
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
		_gameObject.transform.position.x,
		_gameObject.transform.position.y,
		_gameObject.transform.position.z
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
			glm::radians(_gameObject.transform.rotation.x),
			glm::radians(_gameObject.transform.rotation.y),
			glm::radians(_gameObject.transform.rotation.z)
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
