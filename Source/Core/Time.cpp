#include "Time.hpp"
#include <cmath>
#include <Core/Engine.hpp>
#include <iostream>

using namespace TM::Core;

float Time::totalTime = 0.0f;

Time::Time()
{
	_lastFrameTime = std::chrono::high_resolution_clock::now();
}

void Time::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - _lastFrameTime;
	_deltaTime = elapsedTime.count();
	_lastFrameTime = currentTime;

	++_frameCount;
	_timePassed += _deltaTime;
	totalTime += _deltaTime;

	if (_timePassed >= 1.0f) {
		_fps = std::floor(_frameCount / _timePassed);
		_frameCount = 0;
		_timePassed = 0.0f;
	}
}
