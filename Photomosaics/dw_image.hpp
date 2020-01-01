#pragma once

#ifndef __DW_IMAGE__
#define __DW_IMAGE__

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <random>

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
  static void download_img(const std::string&, int, unsigned = 0, unsigned = 0);
  static void clear_img(const std::string&);
  static void create_dir(const std::string&);
};

#endif