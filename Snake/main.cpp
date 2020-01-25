#include <unistd.h>

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <thread>       // sleep_for
#include <chrono>       // microseconds

#include "./src/game.hpp"

std::size_t GameBase::m_width = 100;
std::size_t GameBase::m_height = 24;

int main()
{
    Game game;
    Snake snake;
    Apple apple;

    game.render();
    snake.render();
    apple.render();

    while (true) {
        if (snake.is_dead())
            game.terminate();

        if (snake.head() == apple.get_apple()) {
            snake.increment_body();
            apple.clear();
            apple.update();
            apple.render();
        }

        snake.print_head();
        snake.clear();
        snake.take_step();
        snake.change_direction(Game::detect_key());
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}