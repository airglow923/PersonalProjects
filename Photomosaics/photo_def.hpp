#pragma once

#if !defined(Photo_def)
#define Photo_def

#include <fstream>
#include <array>

#define DIR     "images"
#define SRC     100     // source images
#define BLOCKS  50      // number of blocks per side

struct Piece {
  unsigned width;
  unsigned height;
};

struct RGB {
  unsigned R;
  unsigned G;
  unsigned B;
};

inline std::ostream& operator<<(std::ostream& os, const struct RGB& rgb)
{
  os << rgb.R << ", " << rgb.G << ", " << rgb.B;
  return os;
}

#endif