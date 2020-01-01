#pragma once

#ifndef __PHOTOMOSAICS__
#define __PHOTOMOSAICS__

#include <cstdio>
#include <cstdlib>
#include <cmath>    // round

#ifdef _WIN32
  #include <Windows.h>
#else
  #include <unistd.h>
#endif

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <iomanip>    // setw
#include <array>

#if __GNUC__ >= 8
# include <filesystem>
  namespace fs = std::filesystem;
#else
# include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

#include <ImageMagick-7/Magick++.h>

#include "dw_image.hpp"

const std::string DIR = "./images/";
#define SRC     100
#define BLOCKS  250

struct Piece {
  unsigned width;
  unsigned height;
};

struct RGB {
  unsigned R;
  unsigned G;
  unsigned B;
};

class Photomosaics {
public:
  Photomosaics() = default;
  Photomosaics(const std::string&);
  void load_img(const std::string&);
  void disp_color_map();
private:
  void adjust_piece();
  static struct RGB calc_avg_color(
    Magick::Image, unsigned, unsigned, unsigned, unsigned);
  void mosaicify();
  static double calc_color_difference(
    const struct RGB&, const struct RGB&);
  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;
  struct Piece piece;
  std::vector<std::array<struct RGB, BLOCKS>> color_map;
  static unsigned src;
};

#endif