#include "GameOfLife.hpp"
#include "LangtonsAnt.hpp"

LAnt::LAnt(int width_, int height_)
  : width(width_), height(height_), d(Direction::Forward) {
    std::srand(std::time(0));

    ant_x = std::rand() % width_;
    ant_y = std::rand() % height_;
  }

// when cell is 0 or dead
void LAnt::turn_left(int** arr)
{
  switch(d) {
    case Direction::Forward:
      if (ant_x - 1 >= 0)
        ant_x--;
      d = Direction::Left;
      break;

    case Direction::Right:
      if (ant_y - 1 >= 0)
        ant_y--;
      d = Direction::Forward;
      break;

    case Direction::Backward:
      if (ant_x + 1 < width)
        ant_x++;
      d = Direction::Right;
      break;

    case Direction::Left:
      if (ant_y + 1 < height)
        ant_y++;
      d = Direction::Backward;
      break;
  }
}

// when cell is 1 or alive
void LAnt::turn_right(int** arr)
{
  switch (d) {
    case Direction::Forward:
      if (ant_x + 1 < width)
        ant_x++;
      d = Direction::Right;
      break;

    case Direction::Right:
      if (ant_y + 1 < height)
        ant_y++;
      d = Direction::Backward;
      break;

    case Direction::Backward:
      if (ant_x - 1 >= 0)
        ant_x--;
      d = Direction::Left;
      break;

    case Direction::Left:
      if (ant_y - 1 >= 0)
        ant_y--;
      d = Direction::Forward;
      break;
  }
}

void LAnt::ant_move(int** arr)
{
  int tmp_x = ant_x;
  int tmp_y = ant_y;

  if (arr[ant_x][ant_y])
    turn_right(arr);
  else
    turn_left(arr);

  arr[tmp_x][tmp_y] = arr[tmp_x][tmp_y] ? 0 : 1;
}