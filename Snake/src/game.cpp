#include "game.hpp"

Game::Game()
{
    system("stty -echo");
}

void Game::render() const
{
    std::cout << '+';
    for (std::size_t w = 0; w < m_width - 2; w++)
        std::cout << '-';

    std::cout << "+\n";

    for (std::size_t h = 0; h < m_height - 2; h++) {
        std::cout << '|';
        for (std::size_t w = 0; w < m_width - 2; w++)
            std::cout << ' ';
        std::cout << "|\n";
    }

    std::cout << '+';
    for (std::size_t w = 0; w < m_width - 2; w++)
        std::cout << '-';

    std::cout << '+' << std::flush;
}

void Game::clear() const
{
    for (std::size_t h = 2; h < m_height - 2; h++) {
        MVCURSOR(m_width - 1, h);
        std::cout << "\r|" << std::flush;
    }
}

void Game::terminate() const
{
    MVCURSOR(1, m_height);
    std::cout << std::endl;
    system("stty echo");
    exit(EXIT_SUCCESS);
}

bool Game::isPressed(char* keys_return, KeyCode key)
{
    return !!(keys_return[key >> 3] & (1 << (key & 7)));
}

Direction Game::detect_key()
{
    Display* dpy = XOpenDisplay(":0");
    char keys_return[32];

    XQueryKeymap(dpy, keys_return);
    KeyCode left    = XKeysymToKeycode(dpy, XK_Left);
    KeyCode up      = XKeysymToKeycode(dpy, XK_Up);
    KeyCode right   = XKeysymToKeycode(dpy, XK_Right);
    KeyCode down    = XKeysymToKeycode(dpy, XK_Down);
    
    XCloseDisplay(dpy);

    if (isPressed(keys_return, left))
        return Direction::LEFT;
    else if (isPressed(keys_return, up))
        return Direction::UP;
    else if (isPressed(keys_return, right))
        return Direction::RIGHT;
    else if (isPressed(keys_return, down))
        return Direction::DOWN;
    else
        return Direction::NONE;
}