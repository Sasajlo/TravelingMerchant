#pragma once

#include <cmath>

namespace TM
{
	namespace Utils
	{
		class Size
		{
		public:
			int width, height;

			// Constructors
			Size() : width(0.0f), height(0.0f) {}
			Size(int width, int height) : width(width), height(height) {}
			Size(int value) : width(value), height(value) {}

			// Operators
			Size operator+(const Size& other) const { return Size(width + other.width, height + other.height); }
			Size operator-(const Size& other) const { return Size(width - other.width, height - other.height); }
			Size operator*(float scalar) const { return Size(width * scalar, height * scalar); }
			Size operator/(float scalar) const { return Size(width / scalar, height / scalar); }

			Size& operator+=(const Size& other) { width += other.width; height += other.height; return *this; }
			Size& operator-=(const Size& other) { width -= other.width; height -= other.height; return *this; }
			Size& operator*=(int scalar) { width *= scalar; height *= scalar; return *this; }
			Size& operator/=(int scalar) { width /= scalar; height /= scalar; return *this; }

			bool operator==(const Size& other) const { return width == other.width && height == other.height; }
			bool operator!=(const Size& other) const { return !(*this == other); }
		};
	}
}