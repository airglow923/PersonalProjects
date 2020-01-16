#include <iostream>

#include "game.hpp"
#include "snake.hpp"
#include "apple.hpp"

int main()
{
    Game game(100, 24);
    game.render();
}