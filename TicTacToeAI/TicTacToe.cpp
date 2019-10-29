#include "TicTacToe.hpp"

TAI::TAI()
{
  board = make_board();
}

TAI::TAI(const TAI& tai)
  : board(tai.board)
{
}

TAI::TAI(TAI&& tai)
  : board(std::move(tai.board))
{
}

TAI::~TAI()
{
}

void TAI::render()
{
  std::cout << "  0 1 2  \n  -----\n";
  for (unsigned i = 0; i < 3; i++) {
    std::cout << i << "|";
    for (unsigned j = 0; j < 3; j++)
      std::cout << (board[i][j] ? "X" : "O") << (j == board[i].size() - 1 ? "|" : " ");
    std::cout << (i == board[i].size() - 1 ? "\n  -----\n" : "\n");
  }
}

std::vector<std::vector<bool>> TAI::make_board()
{
  std::vector<std::vector<bool>> board;

  board.reserve(3);
  board = {
    {false, false, false},
    {false, false, false},
    {false, false, false}
  };

  return board;
}