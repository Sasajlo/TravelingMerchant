#pragma once

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Scene.hpp"
#include "Time.hpp"

namespace TM
{
    namespace Core
    {
        class Engine
        {
        public:
            Engine();
            ~Engine();

            bool Initialize(const std::string& title, int width, int height);
            void Run();
            void Stop();
            void Shutdown();

        private:
			Window _window{};
			Time _time;
        };
    }
}