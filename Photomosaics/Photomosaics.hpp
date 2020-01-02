#pragma once

#ifndef __PHOTOMOSAICS__
#define __PHOTOMOSAICS__

#include <cstdio>
#include <cstdlib>
#include <cmath>    // round

#ifdef _WIN32
# include <Windows.h>
#else
# include <unistd.h>
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

const std::string DIR = "images";
#define SRC     100
#define BLOCKS  50

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
  static void load_src();
  void mosaicify(const std::string&);

  struct Piece piece;
private:
  void adjust_piece();

  static struct RGB calc_avg_color(
    Magick::Image, unsigned, unsigned, unsigned, unsigned);
  static struct RGB calc_avg_color(const std::string&);
  
  void pixellate();
  void build_block_map();
  static double calc_color_difference(
    const struct RGB&, const struct RGB&);

  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;

public:
  std::vector<std::array<struct RGB, BLOCKS>> color_map;
  std::vector<std::array<unsigned, BLOCKS>> block_map;
  static std::array<struct RGB, SRC> src_color_map;
};

#endif