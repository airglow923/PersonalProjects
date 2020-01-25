#include "snake.hpp"

Snake::Snake()
{
    m_body.reserve((m_width - 2) * (m_height -2));
    init_body();
    m_direction = static_cast<Direction>(new_direction());
}

Snake::Snake(std::initializer_list<std::pair<std::size_t, std::size_t>> il)
{
    m_body.reserve((m_width - 2) * (m_height -2));

    for (const auto& pos : il)
        m_body.push_back(pos);
}

void Snake::init_body()
{
    m_body.emplace_back(
        generate_pos(2, m_width - 2),
        generate_pos(2, m_height - 2));
}

std::size_t Snake::has_at(std::size_t x, std::size_t y) const
{
    for (std::size_t i = 0; i < m_body.size(); i++)
        if (m_body[i].first == x && m_body[i].second == y)
            return i;
    return -1;
}

void Snake::render() const
{
    for (const auto& pos : m_body) {
        MVCURSOR(pos.first, pos.second);
        std::cout << 'O' << std::flush;
    }
}

void Snake::clear() const
{
    MVCURSOR(tail().first, tail().second);
    std::cout << ' ' << std::flush;
}

void Snake::print_head() const
{
    MVCURSOR(head().first, head().second);
    std::cout << 'O' << std::flush;
}

void Snake::change_direction(Direction direction)
{
    if (direction != Direction::NONE)
        m_direction = direction;
}

std::pair<std::size_t, std::size_t>
Snake::apply_change(std::size_t x, std::size_t y, Direction direction)
{
    switch(direction) {
    case Direction::NONE:
        break;
    case Direction::LEFT:
        x -= 1;
        break;
    case Direction::UP:
        y -= 1;
        break;
    case Direction::RIGHT:
        x += 1;
        break;
    case Direction::DOWN:
        y += 1;
        break;
    }

    return {x, y};
}

void Snake::take_step()
{
    auto prv = m_body[0];

    m_body[0] = apply_change(prv.first, prv.second, m_direction);

    for (std::size_t i = 1; i < m_body.size(); i++) {
        auto cur = m_body[i];
        m_body[i] = prv;
        prv = cur;
    }
}

void Snake::increment_body()
{
    std::size_t x = tail().first;
    std::size_t y = tail().second;

    m_body.push_back(apply_change(x, y, m_direction));
}

bool Snake::contact_body() const
{
    for (std::size_t i = 1; i < m_body.size(); i++)
        if (head() == m_body[i])
            return true;
    return false;
}

bool Snake::contact_body(
    std::pair<std::size_t, std::size_t> head,
    const std::vector<std::pair<std::size_t, std::size_t>>& body)
{
    for (std::size_t i = 1; i < body.size(); i++)
        if (head == body[i])
            return true;
    return false;
}

bool Snake::contact_wall() const
{
    return head().first == 1 || head().first == m_width ||
           head().second == 1 || head().second == m_height;
}

bool Snake::is_dead() const
{
    return contact_body() || contact_wall();
}

std::size_t Snake::generate_pos(std::size_t lower, std::size_t upper)
{
    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution<std::mt19937::result_type> range(lower, upper);

    return range(seed);
}

unsigned Snake::new_direction()
{
    return generate_pos(0, 4);
}