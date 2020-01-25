#pragma once

#if !defined(Apple_header)
#define Apple_header

#include <iostream>
#include <random>
#include <utility>

#include "game_base.hpp"

class Apple : public GameBase {
public:
    Apple();
    ~Apple() {};

    void render() const override;
    void clear() const override;
    void update();
    std::pair<std::size_t, std::size_t> get_apple() const
    {return m_apple;}

private:
    static std::size_t generate_pos(std::size_t, std::size_t);
    
    std::pair<std::size_t, std::size_t> m_apple;
};

#endif