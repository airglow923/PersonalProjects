#include "TicTacToe.hpp"
#include "Extension.hpp"

using TAI = TicTacToe;
using ext = Extension;

int main()
{
  TAI tai;
  int x, y;
  char mark;

  // tai.render();
  // std::cout << "Enter input (x-coordinate, y-coordinate, mark): ";
  // while (std::cin >> x >> y >> mark) {
  //   tai.make_move(x, y, mark);
  //   tai.render();
  //   auto result = tai.is_won();
  //   if (result.first) {
  //     std::cout << "Player " << result.second << " won.\n";
  //     break;
  //   }
  //   if (tai.is_full()) {
  //     std::cout << "Draw\n";
  //     break;
  //   }
  //   std::cout << "Enter input (x-coordinate, y-coordinate, mark): ";
  // }

  tai.render();
  std::cout << "Enter input (x-coordinate, y-coordinate, mark): ";
  while (std::cin >> x >> y) {
    tai.make_move(x, y, 'O');
    tai.render();

    auto result = tai.has_won();
    if (result.first) {
      std::cout << "Player " << result.second << " won.\n";
      break;
    }
    if (tai.is_full()) {
      std::cout << "Draw\n";
      break;
    }

    std::cout << "AI's turn.\n";
    auto ai = ext::minimax_move(tai, 'X');
    tai.make_move(ai, 'X');
    tai.render();

    result = tai.has_won();
    if (result.first) {
      std::cout << "Player " << result.second << " won.\n";
      break;
    }
    if (tai.is_full()) {
      std::cout << "Draw\n";
      break;
    }
    std::cout << "Enter input (x-coordinate, y-coordinate, mark): ";
  }
}