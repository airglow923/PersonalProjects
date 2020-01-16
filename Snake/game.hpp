#pragma once

#if !defined(Game_header)
#define Game_header

#include <iostream>

class Game {
public:
    Game() = default;
    Game(unsigned, unsigned);

    void render() const;
private:
    unsigned width;
    unsigned height;
};

#endif