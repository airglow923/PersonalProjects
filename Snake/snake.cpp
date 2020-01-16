#include "snake.hpp"

Snake::Snake(unsigned width, unsigned height)
{
    body.reserve((width - 2) * (height -2));
}

void Snake::init_body()
{

}

void Snake::init_direction()
{
    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution<std::mt19937::result_type> range(0, 4);

    direction = static_cast<Direction>(range(seed));
}