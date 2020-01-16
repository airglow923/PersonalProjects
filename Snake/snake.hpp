#pragma once

#if !defined(Snake_header)
#define Snake_header

#include <utility>      // pair
#include <vector>
#include <random>

enum class Direction {
    UP, RIGHT, DOWN, LEFT
};

class Snake {
public:
    Snake() = default;
    Snake(unsigned, unsigned);

    void init_body();
    void init_direction();
    void take_step(const std::pair<Direction, Direction>&);

private:
    std::vector<std::pair<unsigned, unsigned>> body;
    Direction direction;
};

#endif