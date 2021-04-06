#pragma once

#include "game_window.hpp"

namespace game {
    class FirstApp{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

    private:
        GameWindow gameWindow{WIDTH, HEIGHT, "Hello Vulkan"};
    };
}
