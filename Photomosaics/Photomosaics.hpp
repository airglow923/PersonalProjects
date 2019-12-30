#pragma once

#ifndef __PHOTOMOSAICS__
#define __PHOTOMOSAICS__

#include <cstdio>
#include <cstdlib>

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
#define SRC 250

struct piece {
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
  static void download_src_img();
private:
  void create_src_dir();
  void adjust_src();
  static struct RGB calc_avg_color();
  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;
  struct piece src;
  std::vector<struct RGB> color_map;
  static unsigned src;
};

#endif