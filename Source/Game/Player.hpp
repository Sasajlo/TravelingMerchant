#pragma once

#include <iostream>
#include <Core/Core.hpp>
#include <Graphics/Camera.hpp>
#include <Graphics/SpriteRenderer.hpp>
#include <Graphics/TextRenderer.hpp>
#include <Physics/Raycast.hpp>
#include <Game/Interactable.hpp>
#include <Game/SpriteManager.hpp>

//using namespace TM::Core;
using namespace TM::Graphics;
using namespace TM::Physics;

namespace TM
{
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
            SpriteManager* _spriteManager = nullptr;

            Interactable* _hoveredInteractable = nullptr;
            bool _interacting = false;

            // Movement data for other components to read
            Vector3 _currentMovementDirection = Vector3::Zero;
            Vector3 _worldMovementDirection = { 1.0f, 0.0f, 0.0f };
            bool _wasMoving = false;

            Inventory* _inventory = nullptr;

        public:
			Player(GameObject& gameObject) : Component(gameObject) {}

            void Awake() override
            {
                _camera = Camera::GetMain();
                _cursorText = GameObject::Find("Cursor Text")->GetComponent<TextRenderer>();
                _spriteManager = _gameObject.GetComponent<SpriteManager>();
                _inventory = _gameObject.GetComponent<Inventory>();

                // Add animation states
                _spriteManager->AddAnimationState("idle", "Assets/Textures/Player/Animations/idle.png", 4, 4, 5.0f, true);
                _spriteManager->AddAnimationState("run", "Assets/Textures/Player/Animations/run.png", 6, 4, 8.5f, true);
                _spriteManager->AddAnimationState("chop", "Assets/Textures/Player/Animations/chop.png", 4, 4, 8.0f);
                _spriteManager->AddAnimationState("mine", "Assets/Textures/Player/Animations/mine.png", 4, 4, 8.0f);
                _spriteManager->AddAnimationState("harvest", "Assets/Textures/Player/Animations/harvest.png", 4, 4, 8.0f);

                // Add animation triggers
                _spriteManager->SetBool("isMoving", false);
                //_spriteManager->SetTrigger("isChopping");

                // Add animation transitions
                _spriteManager->AddTransition("idle", "run", "isMoving");
                _spriteManager->AddTransition("idle", "chop", "chop");
                _spriteManager->AddTransition("idle", "mine", "mine");
                _spriteManager->AddTransition("idle", "harvest", "harvest");

                _spriteManager->AddTransition("run", "idle", "!isMoving");
                /*_spriteManager->AddTransition("run", "chop", "chop");
                _spriteManager->AddTransition("run", "mine", "mine");
                _spriteManager->AddTransition("run", "harvest", "harvest");*/

                _spriteManager->AddTransition("chop", "idle");
                _spriteManager->AddTransition("mine", "idle");
                _spriteManager->AddTransition("harvest", "idle");
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

                if (!_interacting)
                {
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
                        if (!_wasMoving)
                        {
                            _wasMoving = true;
                            _spriteManager->SetBool("isMoving", true);
                        }
                    }
                    else if (_wasMoving && !_goToTarget)
                    {
                        _wasMoving = false;
                        _spriteManager->SetBool("isMoving", false);
                    }

                    // Store the input direction
                    _currentMovementDirection = direction;

                    float radians = glm::radians(-_cameraAngle);
                    float sinA = std::sin(radians);
                    float cosA = std::cos(radians);

                    Vector3 rotatedDir;
                    rotatedDir.x = direction.x * cosA - direction.z * sinA;
                    rotatedDir.z = direction.x * sinA + direction.z * cosA;
                    rotatedDir.y = 0.0f; // No vertical movement

                    // Store the world-space direction
                    if (rotatedDir != Vector3::Zero)
                        _worldMovementDirection = rotatedDir;

                    _gameObject.transform.position += rotatedDir * MOVEMENT_SPEED * deltaTime;

                }
                else
                {
                    Interact();
                }

                if (!_interacting && Input::IsKeyHeld(GLFW_KEY_SPACE))
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
                        auto interactable = closest->GetComponent<Interactable>();
                        if (interactable->IsValid()) CollectResource(interactable);
                    }
                }

                // Update cursor text
                if (_cursorText)
                {
                    _cursorText->GetGameObject()->transform.position = {
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

                    if (!_interacting && Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
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

				// Handle moving to target position
                if (!_interacting && _goToTarget)
                {
                    Vector3 currentPosition = _gameObject.transform.position;
                    Vector3 directionToTarget = _targetPosition - currentPosition;
                    float interactionDistance = _targetInteractable ? _targetInteractable->GetDistance() : 0.0f;
                    if (directionToTarget.Length() <= interactionDistance)
                    {
                        if (_targetInteractable)
                        {
                            _spriteManager->SetBool("isMoving", false);
                            _targetInteractable->StartInteraction(this); // Collect resource when close enough
						}

                        _goToTarget = false; // Stop moving when close enough
                        _wasMoving = false;
                    }
                    else
                    {
                        directionToTarget = directionToTarget.Normalized();
                        _gameObject.transform.position += directionToTarget * MOVEMENT_SPEED * deltaTime;
                        _worldMovementDirection = directionToTarget;

                        if (!_wasMoving)
                        {
                            _wasMoving = true;
                            _spriteManager->SetBool("isMoving", true);
                        }
                    }
				}

                _spriteManager->SetDirection(_worldMovementDirection);
            }

            void Chop(Interactable* object)
            {
                _spriteManager->SetTrigger("chop");
                _targetInteractable = object;
                _interacting = true;
            }

            void Mine(Interactable* object)
            {
                _spriteManager->SetTrigger("mine");
                _targetInteractable = object;
                _interacting = true;
            }

            void Harvest(Interactable* object)
            {
                _spriteManager->SetTrigger("harvest");
                _targetInteractable = object;
                _interacting = true;
            }

            void Interact()
            {
                if (_spriteManager->IsFinished())
                {
                    StopInteraction();

                    return;
                }

                if (!_targetInteractable || !_targetInteractable->GetGameObject()) return;

                if (_targetInteractable->GetGameObject()->HasTag("Tree"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.25)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Stone"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.25)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Berry Bush"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.2)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Plant"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.2)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Log"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.2)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Rocks"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.2)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
                else if (_targetInteractable->GetGameObject()->HasTag("Slime"))
                {
                    if (_spriteManager->GetAnimationLifeTime() >= 0.25)
                    {
                        if (_targetInteractable == _hoveredInteractable)
                            _hoveredInteractable = nullptr;

                        _targetInteractable->Interact(this);
                        _targetInteractable = nullptr;
                    }
                }
            }

            void StopInteraction()
            {
                _targetInteractable = nullptr;
                _interacting = false;
            }

            void CollectResource(Interactable* resource)
            {
				if (!resource) return;
				_targetInteractable = resource;
				MoveTo(resource->GetGameObject()->transform.position);
			}

            float GetAnimationLifeTime()
            {
                return _spriteManager->GetAnimationLifeTime();
            }

            void AddItem(Item item)
            {
                _inventory->AddItem(item);
            }

            void MoveTo(Vector3 targetPosition)
            {
                _targetPosition = targetPosition;
                _targetPosition.y = 0;
                _goToTarget = true;
            }

            Vector3 GetMovementDirection() const { return _currentMovementDirection; }
            Vector3 GetWorldMovementDirection() const { return _worldMovementDirection; }
            float GetCameraAngle() const { return _cameraAngle; }
        };
    }
}