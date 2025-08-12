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

            static bool Initialize(const std::string& title, int width, int height, bool fullscreen=false);
            static void Run();
            static void Stop();
            static void Shutdown();
            static int GetFPS() { return Instance()._time.GetFPS(); }
            static void RenderFPS(int fps);
            static Size GetWindowSize() { return { Instance()._window.GetWidth(), Instance()._window.GetHeight() }; }
            static void ToggleFullscreen() { return Instance()._window.ToggleFullscreen(); }
        };
    }
}