#include "apple.hpp"

Apple::Apple()
{
    update();
}

void Apple::render() const
{
    MVCURSOR(m_apple.first, m_apple.second);
    std::cout << '@' << std::flush;
}

void Apple::clear() const
{
    MVCURSOR(m_apple.first, m_apple.second);
    std::cout << ' ' << std::flush;
}

void Apple::update()
{
    m_apple = {
        generate_pos(2, m_width - 2),
        generate_pos(2, m_height - 2)
    };
}

std::size_t Apple::generate_pos(std::size_t lower, std::size_t upper)
{
    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution<std::mt19937::result_type> range(lower, upper);

    return range(seed);
}