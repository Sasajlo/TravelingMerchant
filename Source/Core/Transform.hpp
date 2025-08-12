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
			Vector3 position;
			Vector3 rotation; // In degrees: pitch, yaw, roll
			Vector3 scale;

			Transform(GameObject& gameObject) : Component(gameObject), position(Vector3::Zero), rotation(Vector3::Zero), scale(Vector3::One) {}

			// Position methods
			void SetPosition(float x, float y, float z) { position = Vector3(x, y, z); }
			void SetPosition(const Vector3& position) { this->position = position; }
			const Vector3& GetPosition() const { return position; }
			Vector3& GetPosition() { return position; }

			// Rotation methods (in degrees)
			void SetRotation(float pitch, float yaw, float roll) { rotation = Vector3(pitch, yaw, roll); }
			void SetRotation(const Vector3& rotation) { this->rotation = rotation; }
			const Vector3& GetRotation() const { return rotation; }
			Vector3& GetRotation() { return rotation; }

			// Scale methods
			void SetScale(float x, float y, float z) { scale = Vector3(x, y, z); }
			void SetScale(const Vector3& scale) { this->scale = scale; }
			const Vector3& GetScale() const { return scale; }
			Vector3& GetScale() { return scale; }

			// Utility methods
			void Translate(const Vector3& translation) { position += translation; }
			void Rotate(const Vector3& rotation) { this->rotation += rotation; }
			void Scale(const Vector3& scale) { this->scale.x *= scale.x; this->scale.y *= scale.y; this->scale.z *= scale.z; }

			// World transform methods
			Vector3 GetWorldPosition() const;
			Vector3 GetWorldRotation() const;
			Vector3 GetWorldScale() const;
			glm::mat4 GetWorldMatrix() const;

			// Local transform matrix (relative to parent)
			glm::mat4 GetLocalMatrix() const
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
				model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
				return model;
			}

			glm::mat4 GetModelMatrix() const
			{
				return GetWorldMatrix();
			}
		};
	}
}