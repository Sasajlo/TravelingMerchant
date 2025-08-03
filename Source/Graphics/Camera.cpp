#include <Graphics/Camera.hpp>
#include <Core/GameObject.hpp>
#include <Core/Input.hpp>
#include <Game/Player.hpp>

using namespace TM::Graphics;
using namespace TM::Core;
using namespace TM::Game;

Camera* Camera::_mainCamera = nullptr; // Initialize static member

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
