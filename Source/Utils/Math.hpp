#pragma once

#include <random>
#include <chrono>

namespace TM
{
    namespace Utils
    {
        class Math
        {
        public:
            // Initialize random number generator
            static void Initialize();
            
            // Generate random float in range [min, max]
            static float RandomFloat(float min, float max);
            
            // Generate random float in range [0, 1]
            static float RandomFloat();
            
            // Generate random integer in range [min, max] (inclusive)
            static int RandomInt(int min, int max);
            
            // Generate random boolean
            static bool RandomBool();
            
            // Clamp value between min and max
            static float Clamp(float value, float min, float max);
            
            // Linear interpolation between a and b
            static float Lerp(float a, float b, float t);
            
            // Smooth interpolation (smoothstep)
            static float SmoothStep(float edge0, float edge1, float x);

        private:
            static std::mt19937 _randomEngine;
            static std::uniform_real_distribution<float> _floatDistribution;
            static std::uniform_int_distribution<int> _intDistribution;
            static bool _initialized;
        };
    }
}
