#ifndef __GAME_OF_LIFE__
#define __GAME_OF_LIFE__

#include <cstdlib>            // srand, rand, EXIT_FAILURE
#include <ctime>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
// #include <filesystem>      Not working on gcc 9.2, surprisingly
#include <experimental/filesystem>
#include <utility>            // pair, move
#include <initializer_list>

enum class Pattern {
  None,
  Block, Beehive, Loaf, Boat, Tub,        // Still lifes
  Beacon1, Blinker1, Pulsar1, Toad1,      // Oscillators - 1st gen
  Beacon2, Blinker2, Pulsar2, Toad2,      // Oscillators - 2nd gen
  Pulsar3                                 // Oscillators - 3rd gen
};

struct PBoard {
  int state;
  Pattern p;
};

class Extension {
  friend class GameOfLife;
public:
  Extension() = default;
  Extension(std::initializer_list<std::string>);
  Extension(const Extension&);
  Extension(Extension&&);
  ~Extension();
  void readDirectory(std::initializer_list<std::string>);
  bool empty();
private:
  std::vector<std::string> load_pattern(const std::string&);
  int** make_pattern(const std::vector<std::string>&);
  Pattern str_to_pattern(const std::string&);
  void refine_pattern();
  static bool verify_pattern(
    PBoard** pboard, int xcoord, int ycoord, int width);
  static int pattern_prob(double);
  PBoard** make_pboard(int width, int height);
  void assign_pattern(
    PBoard** board, Pattern p, int xcoord, int ycoord, int width_, int height_);void next_board(PBoard**, int, int);

  std::map<
    std::string, std::vector<std::string>
  > patterns;
  std::map<
    Pattern,
    std::pair<int**, int>
  > ref_patterns;
};

class GameOfLife {
  friend class Extension;
public:
  GameOfLife();
  GameOfLife(int, int);
  GameOfLife(int, int, std::initializer_list<std::string>);
  GameOfLife(const GameOfLife&);
  GameOfLife(GameOfLife&&);
  ~GameOfLife();
  static int** dead_state(int width_, int height_);
  static int** random_state(int width_, int height_);
  void random_state_ext();
  void next_state();
  int** getCells() const
  {return cells;}
  void render();
  static void render(int** board, int width, int height);
  void delete_board();
  static void delete_board(int** board, int width);

private:
  static void next_phase(int** initial, const std::pair<int**, int>& pattern);

  int** cells;
  int width;
  int height;
  PBoard** pboard;
};

#endif