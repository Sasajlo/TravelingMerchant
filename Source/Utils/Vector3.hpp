#pragma once

#include <cmath>

namespace TM
{
	namespace Utils
	{
		class Vector3
		{
		public:
			float x, y, z;

			// Constructors
			Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
			Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
			Vector3(float value) : x(value), y(value), z(value) {}

			// Operators
			Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
			Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
			Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
			Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

			Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
			Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
			Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
			Vector3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

			bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
			bool operator!=(const Vector3& other) const { return !(*this == other); }

			// Utility methods
			float Length() const { return sqrt(x * x + y * y + z * z); }
			float SquaredLength() const { return x * x + y * y + z * z; }
			Vector3 Normalized() const { float mag = Length(); return mag > 0 ? *this / mag : Vector3(); }

			// Global constants
			static const Vector3 Zero;
			static const Vector3 One;
			static const Vector3 Forward;
			static const Vector3 Back;
			static const Vector3 Up;
			static const Vector3 Down;
			static const Vector3 Right;
			static const Vector3 Left;
		};
	}
}