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