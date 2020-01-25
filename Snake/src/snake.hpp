#pragma once

#if !defined(Snake_header)
#define Snake_header

#include <iostream>
#include <utility>      // pair
#include <vector>
#include <random>
#include <initializer_list>

#include "game_base.hpp"

class Snake : public GameBase {
public:
    Snake();
    Snake(std::size_t, std::size_t);
    Snake(std::initializer_list<std::pair<std::size_t, std::size_t>>);
    ~Snake() {};

    std::pair<std::size_t, std::size_t> at(std::size_t n) const
    {return m_body.at(n);}

    std::pair<std::size_t, std::size_t> head() const
    {return m_body.front();}
    
    std::pair<std::size_t, std::size_t> tail() const
    {return m_body.back();}

    unsigned size() const
    {return m_body.size();}

    Direction where_to() const
    {return m_direction;}

    std::size_t has_at(std::size_t, std::size_t) const;
    void render() const override;
    void clear() const override;
    void print_head() const;
    void change_direction(Direction);
    void take_step();
    void increment_body();
    bool is_dead() const;

private:
    void init_body();
    bool contact_body() const;
    static bool contact_body(
        std::pair<std::size_t, std::size_t>,
        const std::vector<std::pair<std::size_t, std::size_t>>&);
    bool contact_wall() const;

    static std::size_t generate_pos(std::size_t, std::size_t);
    static unsigned new_direction();
    static std::pair<std::size_t, std::size_t>
    apply_change(std::size_t, std::size_t, Direction);

    std::vector<std::pair<std::size_t, std::size_t>> m_body;
    Direction m_direction;
};

#endif