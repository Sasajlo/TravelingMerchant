#include "Math.hpp"

namespace TM
{
    namespace Utils
    {
        // Static member initialization
        std::mt19937 Math::_randomEngine;
        std::uniform_real_distribution<float> Math::_floatDistribution(0.0f, 1.0f);
        std::uniform_int_distribution<int> Math::_intDistribution(0, 1);
        bool Math::_initialized = false;

        void Math::Initialize()
        {
            if (!_initialized)
            {
                // Seed with current time for better randomness
                auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                _randomEngine.seed(static_cast<unsigned int>(seed));
                _initialized = true;
            }
        }

        float Math::RandomFloat(float min, float max)
        {
            if (!_initialized)
            {
                Initialize();
            }

            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(_randomEngine);
        }

        float Math::RandomFloat()
        {
            if (!_initialized)
            {
                Initialize();
            }

            return _floatDistribution(_randomEngine);
        }

        int Math::RandomInt(int min, int max)
        {
            if (!_initialized)
            {
                Initialize();
            }

            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(_randomEngine);
        }

        bool Math::RandomBool()
        {
            if (!_initialized)
            {
                Initialize();
            }

            return _intDistribution(_randomEngine) == 1;
        }

        float Math::Clamp(float value, float min, float max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        float Math::Lerp(float a, float b, float t)
        {
            return a + t * (b - a);
        }

        float Math::SmoothStep(float edge0, float edge1, float x)
        {
            // Clamp x to [0, 1] range
            x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            
            // Smoothstep function: 3t^2 - 2t^3
            return x * x * (3.0f - 2.0f * x);
        }

        float Math::Sign(float value)
        {
            return value >= 0.0f ? 1.0f : -1.0f;
        }
    }
}
