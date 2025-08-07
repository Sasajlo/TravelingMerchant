#pragma once

#include <iostream>
#include <Core/Component.hpp>
#include <Utils/Vector3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace TM::Utils;

namespace TM
{
	namespace Core
	{
		class Transform : public Component
		{
		public:
			Vector3 _position;
			Vector3 _rotation; // In degrees: pitch, yaw, roll
			Vector3 _scale;

			Transform(GameObject& gameObject) : Component(gameObject), _position(Vector3::Zero), _rotation(Vector3::Zero), _scale(Vector3::One) {}

			// Position methods
			void SetPosition(float x, float y, float z) { _position = Vector3(x, y, z); }
			void SetPosition(const Vector3& position) { _position = position; }
			const Vector3& GetPosition() const { return _position; }
			Vector3& GetPosition() { return _position; }

			// Rotation methods (in degrees)
			void SetRotation(float pitch, float yaw, float roll) { _rotation = Vector3(pitch, yaw, roll); }
			void SetRotation(const Vector3& rotation) { _rotation = rotation; }
			const Vector3& GetRotation() const { return _rotation; }
			Vector3& GetRotation() { return _rotation; }

			// Scale methods
			void SetScale(float x, float y, float z) { _scale = Vector3(x, y, z); }
			void SetScale(const Vector3& scale) { _scale = scale; }
			const Vector3& GetScale() const { return _scale; }
			Vector3& GetScale() { return _scale; }

			// Utility methods
			void Translate(const Vector3& translation) { _position += translation; }
			void Rotate(const Vector3& rotation) { _rotation += rotation; }
			void Scale(const Vector3& scale) { _scale.x *= scale.x; _scale.y *= scale.y; _scale.z *= scale.z; }

			glm::mat4 GetModelMatrix() const
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(_position.x, _position.y, _position.z));
				model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
				model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
				model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
				model = glm::scale(model, glm::vec3(_scale.x, _scale.y, _scale.z));
				return model;
			}
		};
	}
}