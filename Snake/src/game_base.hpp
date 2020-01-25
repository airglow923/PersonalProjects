#pragma once

#if !defined(Game_base_header)
#define Game_base_header

#include <cstddef>
#include <cstdio>

#define DEFAULT_W   80
#define DEFAULT_H   24

#define MVCURSOR(x, y) std::printf("\033[%d;%dH", (int) y, (int) x)

enum class Direction {
    NONE = -1, LEFT, UP, RIGHT, DOWN
};

class GameBase {
public:
    virtual void render() const = 0;
    virtual void clear() const = 0;

protected:
    static std::size_t m_width;      // m_width of terminal
    static std::size_t m_height;     // m_height of terminal
};

#endif