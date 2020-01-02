#pragma once

#ifndef __DW_IMAGE__
#define __DW_IMAGE__

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <random>     // mt19937

#if __GNUC__ >= 8
# include <filesystem>
  namespace fs = std::filesystem;
#else
# include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#endif

#include <curl/curl.h>
// #include <lexbor/html/parser.h>

class DWIMG {
public:
  static void download_img(
    const std::string&,   // path
    int = 0,              // number of images
    unsigned = 0,         // width
    unsigned = 0,         // height
    const std::string& = "https://picsum.photos/seed/");  // url
  static void clear_img(const std::string&);
  static void create_dir(const std::string&);
};

#endif