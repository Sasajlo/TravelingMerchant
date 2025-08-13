#include <Utils/Vector3.hpp>

#include <algorithm>

using namespace TM::Utils;

// Global constants
const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);

Vector3 Vector3::Lerp(Vector3 target, float value)
{
	return Vector3(
		std::lerp(x, target.x, value),
		std::lerp(y, target.y, value),
		std::lerp(z, target.z, value)
	);
}

Vector3 Vector3::Clamp(float min, float max) const
{
	return Vector3(
		std::clamp(x, min, max),
		std::clamp(y, min, max),
		std::clamp(z, min, max)
	);
}

Vector3 Vector3::Clamp(const Vector3& min, const Vector3& max) const
{
	return Vector3(
		std::clamp(x, min.x, max.x),
		std::clamp(y, min.y, max.y),
		std::clamp(z, min.z, max.z)
	);
}


Vector3 Vector3::Cross(const Vector3& other) const
{
	return Vector3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

Vector3 Vector3::RotateAroundY(float angle)
{
	// Create rotation matrix around Y-axis
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	// Apply rotation to vector
	glm::vec4 rotatedVector = rotationMatrix * glm::vec4(x, y, z, 1.0f);

	return Vector3(rotatedVector.x, rotatedVector.y, rotatedVector.z);
}
