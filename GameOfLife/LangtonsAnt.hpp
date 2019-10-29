#ifndef __LANGTONS_ANT__
#define __LANGTONS_ANT_

#include <cstdlib>
#include <ctime>

class LAnt {
public:
  LAnt(int, int);
  void ant_move(int**);

private:
  void turn_left(int**);
  void turn_right(int**);
  enum class Direction {Forward, Right, Backward, Left};

  int ant_x;
  int ant_y;
  int width;
  int height;
  Direction d;
};

#endif