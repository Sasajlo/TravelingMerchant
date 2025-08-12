#pragma once

#include <iostream>
#include <Core/Core.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/TextRenderer.hpp>
#include <Physics/Raycast.hpp>
#include <Game/Interactable.hpp>

//using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Physics;

namespace TM
{
    namespace Core
    {
        class GameObject; // Forward declaration
    }

    namespace Game
    {
        class Player : public Component
        {
        private:
			const Vector3 _scrollOffset = Vector3(0.0f, 2.0f, 1.0f); // Camera offset from player
            const float SCROLL_SPEED = 1.0f; // Player movement speed
            const float MOVEMENT_SPEED = 3.0f; // Player movement speed
			float _cameraAngle = 0.0f;
			float _targetAngle = 0.0f;
			Vector3 _cameraOffset = Vector3(0.0f, 10.0f, 10.0f); // Camera offset from player

			bool _goToTarget = false; // Flag to indicate if the player should move to the target position
			Vector3 _targetPosition = Vector3::Zero; // Target position for camera follow
			Interactable* _targetInteractable = nullptr; // Interactable to interact

			Camera* _camera = nullptr;
            TextRenderer* _cursorText = nullptr;

            Interactable* _hoveredInteractable = nullptr;

        public:
			Player(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
				_camera = Camera::GetMain();
                _cursorText = GameObject::Find("Cursor Text")->GetComponent<TextRenderer>();
            }

            void Start() override
            {

            }

            void Update(float deltaTime) override
            {
				// Handle camera rotation
                if (Input::IsKeyPressed(GLFW_KEY_Q))
                {
                    _targetAngle -= 45.0f;
                }

                if (Input::IsKeyPressed(GLFW_KEY_E))
                {
                    _targetAngle += 45.0f;
                }

                float scrollDelta = Input::GetScrollDelta();
                if (scrollDelta != 0)
                {
                    auto scroll = _scrollOffset * scrollDelta * SCROLL_SPEED;
                    if (_cameraOffset.y - scroll.y >= 2.0f && _cameraOffset.y - scroll.y <= 20.0f)
					    _cameraOffset -= scroll;
                }

                // Lerp camera angle
                _cameraAngle = std::lerp(_cameraAngle, _targetAngle, 10.0f * deltaTime);
                Vector3 newOffset = _cameraOffset.RotateAroundY(_cameraAngle);
                _camera->SetFollowOffset(newOffset);

				// Handle player movement
                Vector3 direction(0.0f, 0.0f, 0.0f);

                if (Input::IsKeyHeld(GLFW_KEY_W))
                    direction.z -= 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_S))
                    direction.z += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_D))
                    direction.x += 1.0f;
                if (Input::IsKeyHeld(GLFW_KEY_A))
                    direction.x -= 1.0f;

                if (direction.Length() > 0.0f) {
					_goToTarget = false; // Stop moving to target if player is moving
                    direction = direction.Normalized();
                }

                float radians = glm::radians(-_cameraAngle);
                float sinA = std::sin(radians);
                float cosA = std::cos(radians);

                Vector3 rotatedDir;
                rotatedDir.x = direction.x * cosA - direction.z * sinA;
                rotatedDir.z = direction.x * sinA + direction.z * cosA;
                rotatedDir.y = 0.0f; // No vertical movement

                _gameObject.transform.position += rotatedDir * MOVEMENT_SPEED * deltaTime;

                // Update cursor text
                if (_cursorText)
                {
                    _cursorText->GetGameObject().transform.position = {
                        Input::GetMousePosition().x + 20,
                        Input::GetMousePosition().y + 50,
                        0
                    };
                }

                // Handle raycast
                auto hit = Raycast::MouseRaycast();
                if (hit.hitObject && hit.hitObject->HasComponent<Interactable>())
                {
                    Interactable* interactable = hit.hitObject->GetComponent<Interactable>();

                    // hover toggle
                    if (_hoveredInteractable != interactable)
                    {
                        if (_hoveredInteractable) _hoveredInteractable->SetHovered(false);
                        interactable->SetHovered(true);
                        _hoveredInteractable = interactable;
                    }

                    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
                    {
                        CollectResource(interactable);
                    }

                    // Show cursor text
                    if (_cursorText)
                    {
                        _cursorText->SetText(interactable->GetInteractionText());
                        _cursorText->SetActive(true);
                    }
                }
                else
                {
                    if (_hoveredInteractable)
                    {
                        _hoveredInteractable->SetHovered(false);
                        _hoveredInteractable = nullptr;
                    }

                    _cursorText->SetActive(false);
                }

                if (hit.hitObject && hit.hitObject->HasTag("Ground"))
                {
                    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
                    {
                        MoveTo(hit.hitPoint);
                    }
                }

                if (Input::IsKeyHeld(GLFW_KEY_SPACE))
                {
                    auto objects = SceneManager::GetActiveScene()->GetActiveGameObjects();

                    GameObject* closest = nullptr;
                    float closestDistSq = std::numeric_limits<float>::infinity();
                    const Vector3 myPos = _gameObject.transform.position;

                    for (auto* go : objects)
                    {
                        if (go == &_gameObject) continue;
                        if (!go->HasComponent<Interactable>()) continue;

                        Vector3 d = go->transform.position - myPos;
                        float distSq = d.SquaredLength();
                        if (distSq < closestDistSq)
                        {
                            closestDistSq = distSq;
                            closest = go;
                        }
                    }

                    const float maxRange = 5.0f; // tweak as needed
                    if (closest && closestDistSq <= maxRange * maxRange)
                    {
                        CollectResource(closest->GetComponent<Interactable>());
                    }
                }

				// Handle moving to target position
                if (_goToTarget)
                {
                    Vector3 currentPosition = _gameObject.transform.position;
                    Vector3 directionToTarget = _targetPosition - currentPosition;
                    if (directionToTarget.Length() < 0.1f)
                    {
                        if (_targetInteractable)
                        {
                            _targetInteractable->Interact(&_gameObject); // Collect resource when close enough
                            _targetInteractable = nullptr; // Clear target resource
						}

                        _goToTarget = false; // Stop moving when close enough
                    }
                    else
                    {
                        directionToTarget = directionToTarget.Normalized();
                        _gameObject.transform.position += directionToTarget * MOVEMENT_SPEED * deltaTime;
                    }
				}
            }

            void CollectResource(Interactable* resource)
            {
				if (!resource) return;
				_targetInteractable = resource;
				MoveTo(resource->GetGameObject().transform.position);
			}

            void MoveTo(Vector3 targetPosition)
            {
                _targetPosition = targetPosition;
                _targetPosition.y = 0;
                _goToTarget = true;
            }
        };
    }
}