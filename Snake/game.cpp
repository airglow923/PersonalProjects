#include "game.hpp"

Game::Game(unsigned width_, unsigned height_)
    : width(width_), height(height_)
{
}

void Game::render() const
{
    for (unsigned w = 0; w < width; w++)
        std::cout << "-";

    std::cout << "\n";
    
    for (unsigned h = 0; h < height - 2; h++) {
        std::cout << "|";
        for (unsigned i = 0; i < width - 2; i++)
            std::cout << " ";
        std::cout << "|\n";
    }

    for (unsigned w = 0; w < width; w++)
        std::cout << "-";

    std::cout << "\n";
}