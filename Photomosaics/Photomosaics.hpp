#pragma once

#ifndef __PHOTOMOSAICS__
#define __PHOTOMOSAICS__

#include <cmath>      // round

#ifdef _WIN32
# include <Windows.h>
#else
# include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>      // setw
#include <array>

#if __GNUC__ >= 8
# include <filesystem>
  namespace fs = std::filesystem;
#else
# include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

#include <ImageMagick-7/Magick++.h>

#include "photo_def.hpp"
#include "dw_image.hpp"
#include "caching.hpp"

class Photomosaics {
public:
  Photomosaics() = default;
  Photomosaics(const std::string&);
  
  void load_img(const std::string&);
  static void load_src();
  void mosaicify(const std::string&);
  
  std::vector<std::array<struct RGB, BLOCKS>>
  get_input_color_map() const
  {return color_map;}

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

  std::vector<std::array<struct RGB, BLOCKS>> color_map;
  std::vector<std::array<unsigned, BLOCKS>> block_map;
public:
  static std::array<struct RGB, SRC> src_color_map;
};

#endif