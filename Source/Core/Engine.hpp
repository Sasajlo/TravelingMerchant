#pragma once

#include <iostream>
#include <memory>

#include "Window.hpp"
#include "Scene.hpp"
#include "Time.hpp"
#include <Utils/Size.hpp>

using namespace TM::Utils;

namespace TM
{
    namespace Core
    {
        class Engine
        {
        private:
            Window _window{};
            Time _time;
            Engine() = default;

            static Engine& Instance()
            {
                static Engine instance;
                return instance;
            }

        public:
            ~Engine() = default;

            static bool Initialize(const std::string& title, int width, int height);
            static void Run();
            static void Stop();
            static void Shutdown();
            static void RenderFPS(int fps);
            static Size GetWindowSize() { return { Instance()._window.GetWidth(), Instance()._window.GetHeight() }; }
        };
    }
}