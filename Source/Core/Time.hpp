#pragma once

#include <chrono>

namespace TM
{
	namespace Core
	{
		class Time
		{
		public:
			Time();
			~Time() = default;

			void Update();
			float GetDeltaTime() const { return _deltaTime; }
			int GetFPS() const { return _fps; }

			static float totalTime;

		private:
			std::chrono::high_resolution_clock::time_point _lastFrameTime;
			float _deltaTime = 0.0f;

			int _fps = 0;
			int _frameCount = 0;
			float _timePassed = 0.0f;
		};
	}
}