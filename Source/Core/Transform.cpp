#include "Transform.hpp"
#include "GameObject.hpp"

using namespace TM::Utils;

namespace TM
{
	namespace Core
	{
		Vector3 Transform::GetWorldPosition() const
		{
			GameObject* parent = _gameObject.GetParent();
			if (parent == nullptr)
			{
				return position;
			}

			// Get parent's world matrix and transform local position
			glm::mat4 parentWorldMatrix = parent->transform.GetWorldMatrix();
			glm::vec4 worldPos = parentWorldMatrix * glm::vec4(position.x, position.y, position.z, 1.0f);
			return Vector3(worldPos.x, worldPos.y, worldPos.z);
		}

		Vector3 Transform::GetWorldRotation() const
		{
			GameObject* parent = _gameObject.GetParent();
			if (parent == nullptr)
			{
				return rotation;
			}

			// For rotation, we add parent's rotation to local rotation
			// This is a simplified approach - for more complex scenarios, quaternions would be better
			return parent->transform.GetWorldRotation() + rotation;
		}

		Vector3 Transform::GetWorldScale() const
		{
			GameObject* parent = _gameObject.GetParent();
			if (parent == nullptr)
			{
				return scale;
			}

			// Scale is multiplicative
			Vector3 parentScale = parent->transform.GetWorldScale();
			return Vector3(
				parentScale.x * scale.x,
				parentScale.y * scale.y,
				parentScale.z * scale.z
			);
		}

		glm::mat4 Transform::GetWorldMatrix() const
		{
			GameObject* parent = _gameObject.GetParent();
			if (parent == nullptr)
			{
				return GetLocalMatrix();
			}

			// Multiply local matrix by parent's world matrix
			return parent->transform.GetWorldMatrix() * GetLocalMatrix();
		}
	}
}
