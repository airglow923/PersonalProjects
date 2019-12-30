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

#if __GNUC__ >= 8
  #include <filesystem>
  namespace fs = std::filesystem;
#else
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

#include "curl/curl.h"
#include "ImageMagick-7/Magick++.h"

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
  Photomosaics(std::string, unsigned, unsigned);
  void load_img(const std::string&);
	void download_sprt_src_img();
  void clear_sprt_src_img();
  static void download_src_img(unsigned, unsigned);
private:
  void create_src_dir();
  void adjust_piece();
  static struct RGB calc_avg_color(const Magick::Image&);
  void mosaicify();
  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;
  struct Piece piece;
  std::vector<std::vector<struct RGB>> color_map;
  static unsigned src;
};

#endif