#pragma once

#if !defined(Caching_header)
#define Caching_header

#include <cstdio>
#include <cstdlib>          // sprintf
#include <cstddef>          // size_t, ptrdiff_t

#include <iostream>
#include <fstream>          // ofstream
#include <string>
#include <filesystem>
#include <tuple>
#include <regex>            // regex_match
#include <array>
#include <iterator>         // distance

#include <sys/stat.h>       // fstat
#include <sys/mman.h>       // mman, munmap
#include <fcntl.h>          // open
#include <openssl/md5.h>    // MD5

#include "photo_def.hpp"

namespace fs = std::filesystem;

class Caching {
public:
  template<std::size_t N>
  static void write_input_json(
    const std::string&,
    const std::string&,
    const std::vector<std::array<struct RGB, N>>&,
    std::ios_base::openmode = std::ios::out);
    
  template<std::size_t N>
  static void read_input_json(
    const std::string&,
    const std::string&,
    std::vector<std::array<struct RGB, N>>&,
    std::ios_base::openmode = std::ios::in);
  
  template<std::size_t N>
  static void write_src_json(
    const std::string&,
    const std::array<struct RGB, N>&,
    std::ios_base::openmode = std::ios::out);

  template<std::size_t N>
  static void read_src_json(
    const std::string&,
    std::array<struct RGB, N>&,
    std::ios_base::openmode = std::ios::in);

  static std::string get_hash(const std::string&);
  static bool check_hash(const std::string&, const std::string&);
};

static std::fstream& insert_tab(std::fstream& fs, const int& n)
{
  fs << std::string(n, '\t');
  return fs;
}

static std::string name_check(const std::string& filename)
{
  fs::path p(filename);

  return !p.extension().compare(".json") ? filename : filename + ".json";
}

static void beautify(std::string& s)
{
  std::string target(R"({[]}",:)");

  for (size_t pos = 0;
       pos < s.size() &&
       (pos = s.find_first_of(target, pos)) != std::string::npos;)
    s.erase(s.begin() + pos);
}

template<std::size_t N>
void Caching::write_input_json(
  const std::string& filename,
  const std::string& target,
  const std::vector<std::array<struct RGB, N>>& objects,
  std::ios_base::openmode mode)
{
  std::fstream fs(name_check(filename), mode);
  int tab = 0;

  if (!fs) {
    std::cerr << "File error\n";
    exit(-1);
  }

  fs << "{\n";
  tab++;

  insert_tab(fs, tab) << "\"hash\": \"" << get_hash(target) << "\",\n";
  insert_tab(fs, tab) << "\"RGB\": [\n";
  tab++;

  for (size_t col = 0; col < objects.size(); col++) {
    for (size_t row = 0; row < objects[0].size(); row++) {
      insert_tab(fs,tab) << "[" << objects[col][row] << "]";
      fs << (col == objects.size() - 1 && row == objects[0].size() - 1 ?
        "\n" : ",\n");
    }
  }

  tab--;
  insert_tab(fs, tab) << "]\n";
  tab--;
  insert_tab(fs, tab) << "}\n";

  fs.close();
}

template<std::size_t N>
void Caching::read_input_json(
  const std::string& filename,
  const std::string& target,
  std::vector<std::array<struct RGB, N>>& blocks,
  std::ios_base::openmode mode)
{
  std::fstream fs(filename, mode);

  if (!fs) {
    std::cerr << "Cannot find file\n";
    exit(1);
  }
  
  for (std::string s; fs >> s;) {
    beautify(s);

    if (s.empty())
      continue;

    if (!s.compare("hash")) {
      fs >> s;
      beautify(s);
      if (s.compare(Caching::get_hash(target))) {
        std::cerr << "Cached data is not from the same file.\n";
        return;
      }
    } else if (!s.compare("RGB")) {
      fs >> s;
      bool quit = false;

      for (std::string r, g, b; fs;) {
        std::array<struct RGB, N> col;
        for (size_t i = 0; i < N; i++) {
          fs >> r >> g >> b;

          beautify(r);
          beautify(g);
          beautify(b);

          if (r.empty() || g.empty() || b.empty()) {
            quit = true;
            break;
          }

          col[i] = {
            (unsigned) std::stoi(r),
            (unsigned) std::stoi(g),
            (unsigned) std::stoi(b)
          };
        }
        if (quit)
          break;
        blocks.push_back(col);
      }
      // for (auto& block : blocks) {
      //   for (auto& row : block) {
      //     std::string r, g, b;
      //     fs >> r >> g >> b;
      //     beautify(r);
      //     beautify(g);
      //     beautify(b);

      //     if (r.empty() || g.empty() || b.empty()) {
      //       quit = true;
      //       break;
      //     }
          
      //     row = {
      //       (unsigned) std::stoi(r),
      //       (unsigned) std::stoi(g),
      //       (unsigned) std::stoi(b)
      //     };
      //   }
      //   if (quit)
      //     break;
      // }
    }
  }

  fs.close();
}

template<std::size_t N>
void Caching::write_src_json(
  const std::string& filename,
  const std::array<struct RGB, N>& objects,
  std::ios_base::openmode mode)
{
  std::fstream fs(name_check(filename), mode);
  int tab = 0;

  if (!fs) {
    std::cerr << "File not found\n";
    exit(-1);
  }

  fs << "{\n";
  tab++;

  for (size_t i = 0; i < objects.size(); i++) {
    insert_tab(fs, tab) << "\"" << i << ".jpg\": [" << objects[i] << "]";
    fs << (i == objects.size() - 1 ? "\n" : ",\n");
  }

  tab--;
  insert_tab(fs, tab) << "}\n";

  fs.close();
}

template<std::size_t N>
void Caching::read_src_json(
  const std::string& filename,
  std::array<struct RGB, N>& blocks,
  std::ios_base::openmode mode)
{
  std::fstream fs(filename, mode);
  std::string s;

  if (!fs) {
    std::cerr << "Cannot find the file.\n";
    exit(1);
  }

  std::string r, g, b;
  fs >> s;

  for (size_t i = 0; fs >> s >> r >> g >> b && i < blocks.size(); i++) {
    beautify(r);
    beautify(g);
    beautify(b);

    if (r.empty() || g.empty() || b.empty())
      break;

    blocks[i] = {
      (unsigned) std::stoi(r),
      (unsigned) std::stoi(g),
      (unsigned) std::stoi(b)
    };
  }

  fs.close();
}

#endif