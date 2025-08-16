#pragma once

namespace TM
{
	namespace Utils
	{
		class Color
		{
		public:
			float r, g, b, a;

			// Constructor
			Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) 
				: r(r), g(g), b(b), a(a) {}

			Color(int r = 255, int g = 255, int b = 255, int a = 255)
				: r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {
			}

			// Static color constants
			static const Color WHITE;
			static const Color GRAY;
			static const Color BLACK;
			static const Color RED;
			static const Color GREEN;
			static const Color BLUE;
			static const Color YELLOW;
			static const Color CYAN;
			static const Color MAGENTA;
			static const Color ORANGE;
			static const Color PURPLE;
		};
	}

}