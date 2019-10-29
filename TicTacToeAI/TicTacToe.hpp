#pragma once

#ifndef __TIC_TAC_TOE_AI__
#define __TIC_TAC_TOE_AI__

#include <cstdlib>

#include <iostream>
#include <vector>
#include <utility>    // move

class TAI {
public:
  TAI();
  TAI(const TAI&);
  TAI(TAI&&);
  ~TAI();
  void render();
private:
  static std::vector<std::vector<bool>> make_board();
  
  std::vector<std::vector<bool>> board;
};

#endif