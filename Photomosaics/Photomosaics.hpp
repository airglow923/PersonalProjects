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

#if __GNUC__ >= 8
  #include <filesystem>
  namespace fs = std::filesystem;
#else
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

#include <Magick++.h>

#include "curl/curl.h"

const std::string DIR = "./images/";
#define SRC 100
#define BLOCKS 250

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
  Photomosaics(const std::string&, unsigned, unsigned);
  void load_img(const std::string&);
	void download_sprt_src_img();
  void clear_sprt_src_img();
  static void download_src_img(unsigned, unsigned);
  void disp_color_map();
private:
  void create_src_dir();
  void adjust_piece();
  static struct RGB calc_avg_color(const Magick::Image&);
  void mosaicify();
  static double calc_color_difference(
    const struct RGB&, const struct RGB&);
  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;
  struct Piece piece;
  std::vector<std::vector<struct RGB>> color_map;
  static unsigned src;
};

#endif