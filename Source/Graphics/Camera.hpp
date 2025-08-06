#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Utils/Color.hpp>
#include <Utils/Vector3.hpp>
#include <glm/glm.hpp>

using namespace TM::Utils;
using namespace TM::Core;

namespace TM
{
	namespace Core
	{
		class GameObject; // Forward declaration
	}

	namespace Graphics
	{
		class Camera : public Component
		{
		private:
			static Camera* _mainCamera; // Static pointer to the main camera instance
			Color _backgroundColor = Color::GRAY; // RGBA clear color
			float _fov = 45.0f; // Field of view in degrees
			float _nearPlane = 0.1f; // Near clipping plane
			float _farPlane = 100.0f; // Far clipping plane

			// Look at target
			GameObject* _followTarget = nullptr;
			GameObject* _lookAtTarget = nullptr;
			Vector3 _followOffset = Vector3(0.0f, 5.0f, 10.0f); // Camera offset from target

		public:
			Camera(GameObject& gameObject);
			static Camera* GetMain() { return _mainCamera; }

			// Override component methods
			void Awake() override;
			void Start() override;
			void Update(float deltaTime) override;

			void SetBackgroundColor(Color color);
			Color GetBackgroundColor();
			void SetMainCamera();

			void LookAt(GameObject* target) { _lookAtTarget = target; }
			void Follow(GameObject* target, Vector3 offset) { _followTarget = target; _followOffset = offset; }

			Vector3 GetFollowOffset() { return _followOffset; }
			void SetFollowOffset(Vector3 offset) { _followOffset = offset; }

			glm::mat4 GetViewMatrix() const;
			glm::mat4 GetProjectionMatrix() const;
		};
	}
}
