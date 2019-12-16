#include "TicTacToe.hpp"

TicTacToe::TicTacToe()
{
  board = make_board();
}

TicTacToe::TicTacToe(unsigned n)
{
  board = make_board(n);
}

TicTacToe::TicTacToe(const TicTacToe& tai)
  : board(tai.board)
{
}

TicTacToe::TicTacToe(TicTacToe&& tai)
  : board(std::move(tai.board))
{
}

TicTacToe::~TicTacToe()
{
}

void TicTacToe::render()
{
  // std::cout << "\n  0 1 2  \n  -----\n";
  std::cout << "\n  ";
  for (unsigned i = 0; i < board.size(); i++)
    std::cout << i << " ";
  std::cout << " \n  ";
  for (unsigned i = 0; i < board.size() * 2 - 1; i++)
    std::cout << "-";
  std::cout << "\n";

  for (unsigned i = 0; i < board.size(); i++) {
    std::cout << i << "|";
    for (unsigned j = 0; j < board[0].size(); j++)
      std::cout << board[i][j] << (j == board[0].size() - 1 ? "|" : " ");
    std::cout << (i != board[i].size() - 1 ? "\n" : "");
  }

  std::cout << " \n  ";
  for (unsigned i = 0; i < board.size() * 2 - 1; i++)
    std::cout << "-";
  std::cout << "\n";
}

void TicTacToe::render(const std::vector<std::vector<char>>& board)
{
  std::cout << "\n  ";
  for (unsigned i = 0; i < board.size(); i++)
    std::cout << i << " ";
  std::cout << " \n  ";
  for (unsigned i = 0; i < board.size() * 2 - 1; i++)
    std::cout << "-";
  std::cout << "\n";

  for (unsigned i = 0; i < board.size(); i++) {
    std::cout << i << "|";
    for (unsigned j = 0; j < board[0].size(); j++)
      std::cout << board[i][j] << (j == board[0].size() - 1 ? "|" : " ");
    std::cout << (i != board[i].size() - 1 ? "\n" : "");
  }

  std::cout << " \n  ";
  for (unsigned i = 0; i < board.size() * 2 - 1; i++)
    std::cout << "-";
  std::cout << "\n";
}

void TicTacToe::make_move(int x, int y, char mark)
{
  if (x < 0 || x > board.size() - 1 || y < 0 || y > board.size() - 1) {
    std::cerr << "Wrong input. Enter number between 0 and 2 inclusive.\n";
    return;
  }

  if (board[y][x] != ' ') {
    std::cerr << "The cell is already occupied.\n";
    return;
  }

  board[y][x] = mark;
}

void TicTacToe::make_move(const std::pair<unsigned, unsigned>& xy, char mark)
{
  make_move(xy.first, xy.second, mark);
}

std::pair<bool, char> TicTacToe::has_won()
{
  for (unsigned row = 0; row < board.size(); row++) {
    char tmp = board[row][0];
    bool win = true;
    for (unsigned col = 1; col < board[0].size(); col++) {
      if (board[row][col] == ' ' || tmp != board[row][col]) {
        win = false;
        break;
      }
    }
    if (win)
      return {true, tmp};
  }

  for (unsigned col = 0; col < board.size(); col++) {
    char tmp = board[0][col];
    bool win = true;
    for (unsigned row = 1; row < board[0].size(); row++) {
      if (board[row][col] == ' ' || tmp != board[row][col]) {
        win = false;
        break;
      }
    }
    if (win)
      return {true, tmp};
  }

  char start1(board[0][0]);
  bool win = true;

  for (unsigned row = 1, col = 1; row < board.size(); row++, col++) {
    if (board[row][col] == ' ' || start1 != board[row][col]) {
      win = false;
      break;
    }
  }

  if (win)
    return {true, start1};

  char start2(board[0][board.size() - 1]);
  win = true;

  for (unsigned row = 1, col = board.size() - 2; row < board.size(); row++, col--) {
    if (board[row][col] == ' ' || start2 != board[row][col]) {
      win = false;
      break;
    }
  }

  return win ? std::make_pair(true, start2) : std::make_pair(false, ' ');
}

bool TicTacToe::is_full() const
{
  // check whether board is full
  for (const auto& row : board)
    for (const auto& col : row)
      if (col == ' ')
        return false;

  return true;
}

std::vector<std::vector<char>> TicTacToe::make_board(unsigned n)
{
  std::vector<std::vector<char>> board;

  board.reserve(n);
  for (unsigned i = 0; i < n; i++)
    board.emplace_back(n, ' ');

  return board;
}