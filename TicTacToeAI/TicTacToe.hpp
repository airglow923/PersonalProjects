#pragma once

#ifndef __TIC_TAC_TOE__
#define __TIC_TAC_TOE__

#include <cstdlib>

#include <iostream>
#include <vector>
#include <utility>    // move, pair

class TicTacToe {
public:
  TicTacToe();
  TicTacToe(unsigned);
  TicTacToe(const TicTacToe&);
  TicTacToe(TicTacToe&&);
  ~TicTacToe();
  void render();
  static void render(const std::vector<std::vector<char>>&);
  void make_move(int x, int y, char mark);
  void make_move(const std::pair<unsigned, unsigned>&, char);
  std::pair<bool, char> has_won();
  bool is_full() const;
  std::vector<std::vector<char>> get_board() const
  {return board;}
private:
  static std::vector<std::vector<char>> make_board(unsigned = 3);
  
  std::vector<std::vector<char>> board;
};

#endif