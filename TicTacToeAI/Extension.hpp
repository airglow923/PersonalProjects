#pragma once

#ifndef __EXTENSION__
#define __EXTENSION__

#include <random>           // uniform_int_distribution
#include <utility>          // pair
#include <algorithm>        // max_element, min_element
#include <limits>           // numeric_limits<>::min

#include "TicTacToe.hpp"

class Extension {
public:
  static std::pair<unsigned, unsigned> random_move(const TicTacToe&);
  static std::pair<unsigned, unsigned> winning_move(const TicTacToe&, char);
  static std::pair<unsigned, unsigned> minimax_move(const TicTacToe&, char);
public:
  static int minimax_score(std::vector<std::vector<char>>&, char, char);
  static std::vector<std::pair<unsigned, unsigned>>
  get_legal_moves(const std::vector<std::vector<char>>&, char);
  static void make_move(
    std::vector<std::vector<char>>&, std::pair<unsigned, unsigned>, char);
  static bool has_won(const std::vector<std::vector<char>>&, char);
  static bool is_full(const std::vector<std::vector<char>>&);
};

#endif