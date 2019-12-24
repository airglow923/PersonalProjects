#pragma once

#ifndef __PHOTOMOSAICS__
#define __PHOTOMOSAICS__

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "curl/curl.h"
#include "ImageMagick-7/Magick++.h"

#ifndef __IMAGE_DIR__
const std::string DIR = "./images/";
#endif

namespace fs = std::experimental::filesystem;

class Photomosaics {
public:
  Photomosaics() = default;
  Photomosaics(std::string, unsigned, unsigned);
	void download_src_imgs();
  void clear_src_img();
private:
  void download_src_img(
    std::mt19937::result_type, unsigned = 0, unsigned = 0);
  std::string filename;
  unsigned img_no;
  unsigned width;
  unsigned height;
};

#endif