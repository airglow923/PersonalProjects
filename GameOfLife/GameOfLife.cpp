#include "GameOfLife.hpp"

Extension::Extension(std::initializer_list<std::string> dirs)
{
  readDirectory(dirs);
  refine_pattern();
}

Extension::Extension(const Extension& ext)
  : patterns(ext.patterns), ref_patterns(ext.ref_patterns)
{
}

Extension::Extension(Extension&& ext)
  : patterns(std::move(ext.patterns)), ref_patterns(std::move(ext.ref_patterns))
{
}

Extension::~Extension()
{
  if (!ref_patterns.empty())
    for (const auto& pattern : ref_patterns)
      GameOfLife::delete_board(pattern.second.first, pattern.second.second);
}

void Extension::readDirectory(std::initializer_list<std::string> strs)
{
  namespace fs = std::experimental::filesystem;

  for (const auto& dir : strs) {
    for (const auto& entry : fs::directory_iterator(dir)) {
      patterns.insert({
        entry.path().filename(), load_pattern(entry.path())
      });
    }
  }
}

bool Extension::empty()
{
  return patterns.empty() && ref_patterns.empty();
}

std::vector<std::string>
Extension::load_pattern(const std::string& filename)
{
  std::ifstream pattern(filename);

  if (!pattern) {
    std::cerr << "Cannot load the pattern\n";
    exit(EXIT_FAILURE);
  }

  std::string line;
  std::vector<std::string> lines;

  while (pattern >> line)
    lines.push_back(line);

  pattern.close();
  return lines;
}

int** Extension::make_pattern(const std::vector<std::string>& pattern)
{
  int** newCells = GameOfLife::dead_state(pattern[0].size(), pattern.size());

  for (int w = 0; w < pattern[0].size(); w++) {
    for (int h = 0; h < pattern.size(); h++) {
      newCells[w][h] = pattern[h][w] == '1' ? 1 : 0;
    }
  }

  return newCells;
}

Pattern Extension::str_to_pattern(const std::string& name)
{
  if (!name.compare("Block"))
    return Pattern::Block;
  else if (!name.compare("Beehive"))
    return Pattern::Beehive;
  else if (!name.compare("Loaf"))
    return Pattern::Loaf;
  else if (!name.compare("Boat"))
    return Pattern::Boat;
  else if (!name.compare("Tub"))
    return Pattern::Tub;
  else if (!name.compare("Beacon1"))
    return Pattern::Beacon1;
  else if (!name.compare("Beacon2"))
    return Pattern::Beacon2;
  else if (!name.compare("Blinker1"))
    return Pattern::Blinker1;
  else if (!name.compare("Blinker2"))
    return Pattern::Blinker2;
  else if (!name.compare("Pulsar1"))
    return Pattern::Pulsar1;
  else if (!name.compare("Pulsar2"))
    return Pattern::Pulsar2;
  else if (!name.compare("Pulsar3"))
    return Pattern::Pulsar3;
  else if (!name.compare("Toad1"))
    return Pattern::Toad1;
  else if (!name.compare("Toad2"))
    return Pattern::Toad2;

  return Pattern::None;
}

void Extension::refine_pattern()
{
  if (patterns.empty())
    return;

  for (const auto& pattern : patterns) {
    ref_patterns.insert(
      {
        str_to_pattern(pattern.first),
        {make_pattern(pattern.second), pattern.second.size()}
      });
  }
}

bool Extension::verify_pattern(
  PBoard** pboard, int xcoord, int ycoord, int width_
)
{
  for (int w = 0; w < width_; w++) {
    for (int h = 0; h < width_; h++) {
      if (pboard[xcoord + w][ycoord + h].p != Pattern::None)
        return false;
    }
  }

  return true;
}

int Extension::pattern_prob(double oscill)
{
  double p = static_cast<double>(std::rand() / static_cast<double>(RAND_MAX));

  return p > oscill ? std::rand() % 5 + 1 : std::rand() % 4 + 6;
}

PBoard** Extension::make_pboard(int width, int height)
{
  std::srand(0);
  PBoard** pboard = new PBoard*[width];
  Pattern p;

  for (int w = 0; w < width; w++) {
    pboard[w] = new PBoard[height];
    for (int h = 0; h < height; h++) {
      pboard[w][h].p = Pattern::None;
      pboard[w][h].state = 0;
    }
  }

  for (int w = 0; w < width; w++) {
    for (int h = 0; h < height; h++) {
      p = (Pattern) pattern_prob(0.5);
      std::cout << (int) p << "\n";
      switch (p) {
        // Still lifes
        case Pattern::Block:
          assign_pattern(pboard, Pattern::Block, w, h, width, height);
          break;
        case Pattern::Beehive:
          assign_pattern(pboard, Pattern::Beehive, w, h, width, height);
          break;
        case Pattern::Loaf:
          assign_pattern(pboard, Pattern::Loaf, w, h, width, height);
          break;
        case Pattern::Boat:
          assign_pattern(pboard, Pattern::Boat, w, h, width, height);
          break;
        case Pattern::Tub:
          assign_pattern(pboard, Pattern::Tub, w, h, width, height);
          break;

        // Oscillators
        case Pattern::Beacon1:
          assign_pattern(pboard, Pattern::Beacon1, w, h, width, height);
          break;
        case Pattern::Blinker1:
          assign_pattern(pboard, Pattern::Blinker1, w, h, width, height);
          break;
        case Pattern::Pulsar1:
          assign_pattern(pboard, Pattern::Pulsar1, w, h, width, height);
          break;
        case Pattern::Toad1:
          assign_pattern(pboard, Pattern::Toad1, w, h, width, height);
          break;
      }
    }
  }

  return pboard;
}

void Extension::assign_pattern(
  PBoard** pboard, Pattern p, int xcoord, int ycoord, int width_, int height_
)
{
  const int p_width = ref_patterns[p].second;

  if (xcoord + p_width < width_ && ycoord + p_width < height_ &&
      verify_pattern(pboard, xcoord, ycoord, p_width)) {
    for (int w = 0; w < p_width; w++) {
      for (int h = 0; h < p_width; h++) {
        pboard[xcoord + w][ycoord + h].state = ref_patterns[p].first[w][h];
        pboard[xcoord + w][ycoord + h].p = p;
      }
      std::cout << "\n";
    }
  }
}

void next_board(PBoard** pboard, int width, int height)
{
  for (int w = 0; w < width; w++) {
    for (int h = 0; h < height; h++) {
      switch (pboard[w][h].p) {
        case Pattern::Beacon1:
        case Pattern::Beacon2:
        case Pattern::Blinker1:
        case Pattern::Blinker2:
        case Pattern::Pulsar1:
        case Pattern::Pulsar2:
        case Pattern::Toad1:
        case Pattern::Toad2:
        case Pattern::Pulsar3:
          break;
      }
    }
  }
}

GameOfLife::GameOfLife()
  : cells(nullptr), width(0), height(0), pboard(nullptr)
{
}

GameOfLife::GameOfLife(int width_, int height_)
  : width(width_), height(height_), pboard(nullptr)
{
  cells = random_state(width_, height_);
}

GameOfLife::GameOfLife(int width_, int height_,
  std::initializer_list<std::string> dirs)
  : width(width_), height(height_)
{
  Extension e(dirs);
  pboard = e.make_pboard(width_, height_);
  random_state_ext();
}

GameOfLife::GameOfLife(const GameOfLife& GOL)
  : cells(GOL.cells), width(GOL.width), height(GOL.height), pboard(GOL.pboard)
{
}

GameOfLife::GameOfLife(GameOfLife&& GOL)
  : cells(std::move(GOL.cells)), width(GOL.width), height(GOL.height),
    pboard(GOL.pboard)
{
}

GameOfLife::~GameOfLife()
{
  delete_board();
  if (pboard != nullptr) {
    for (int w = 0; w < width; w++)
      delete[] pboard[w];
    delete[] pboard;
  }
}

int** GameOfLife::dead_state(int width_, int height_)
{
  int** board = new int*[width_];

  for (int w = 0; w < width_; w++) {
    board[w] = new int[height_];
    for (int h = 0; h < height_; h++) {
      board[w][h] = 0;
    }
  }

  return board;
}

int** GameOfLife::random_state(int width_, int height_)
{
  std::srand(std::time(0));
  int** board = dead_state(width_, height_);

  for (int w = 0; w < width_; w++) {
    for (int h = 0; h < height_; h++) {
      board[w][h] =
        static_cast<double>(
          std::rand() / static_cast<double>(RAND_MAX)
        ) >= 0.85 ? 1 : 0;
    }
  }

  return board;
}

void GameOfLife::random_state_ext()
{
  std::srand(std::time(0));
  cells = dead_state(width, height);

  for (int w = 0; w < width; w++) {
    for (int h = 0; h < height; h++) {
      cells[w][h] = pboard[w][h].state;
    }
  }
}

void GameOfLife::render()
{
  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      std::cout << (cells[w][h] ? "█" : " ");
    }
    std::cout << "\n";
  }
}

void GameOfLife::render(int** board, int width_, int height_)
{
  for (int h = 0; h < height_; h++) {
    for (int w = 0; w < width_; w++) {
      std::cout << (board[w][h] ? "█" : " ");
    }
    std::cout << "\n";
  }
}

void GameOfLife::next_state()
{
  int** newArr = dead_state(width, height);

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      int lives = 0;

      // first row, first column
      if (w - 1 >= 0 && h - 1 >= 0 && cells[w-1][h-1])
        lives++;

      // first row, second column
      if (w >= 0 && h - 1 >= 0 && cells[w][h-1])
        lives++;

      // first row, thrid column
      if (w + 1 < width && h - 1 >= 0 && cells[w+1][h-1])
        lives++;

      // second row, first column
      if (w - 1 >= 0 && h >= 0 && cells[w-1][h])
        lives++;

      // second row, second column
      // origin

      // second row, thrid column
      if (w + 1 < width && h >= 0 && cells[w+1][h])
        lives++;

      // thrid row, first column
      if (w - 1 >= 0 && h + 1 < height && cells[w-1][h+1])
        lives++;

      // third row, second column
      if (w >= 0 && h + 1 < height && cells[w][h+1])
        lives++;

      // thrid row, thrid column
      if (w + 1 < width && h + 1 < height && cells[w+1][h+1])
        lives++;

      if (lives < 2)
        newArr[w][h] = 0;
      else if (lives > 3)
        newArr[w][h] = 0;
      else if (lives == 3)
        newArr[w][h] = 1;
    }
  }

  delete_board();
  cells = newArr;
}

void GameOfLife::delete_board()
{
  if (cells != nullptr) {
    for (int w = 0; w < width; w++)
      delete[] cells[w];
    delete[] cells;
  }
}

void GameOfLife::delete_board(int** board, int width)
{
  for (int w = 0; w < width; w++)
    delete[] board[w];
  delete[] board;
}