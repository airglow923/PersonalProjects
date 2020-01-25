#pragma once

#if !defined(Game_header)
#define Game_header

#include <cstdlib>

#include <iostream>

#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

#include "game_base.hpp"
#include "snake.hpp"
#include "apple.hpp"

class Game : public GameBase{
public:
    Game();
    Game(std::size_t, std::size_t);
    ~Game() {};

    void render() const override;
    void clear() const override;
    void terminate() const;

    static Direction detect_key();

private:
    static bool isPressed(char*, KeyCode);
};

#endif