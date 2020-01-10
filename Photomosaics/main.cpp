#include <iostream>
#include <iomanip>
#include <filesystem>

#include "Photomosaics.hpp"
#include "dw_image.hpp"
#include "caching.hpp"

namespace fs = std::filesystem;

std::array<struct RGB, SRC> Photomosaics::src_color_map;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Method: " << argv[0] << " [IMAGE]\n";
    exit(1);
  }

  Photomosaics photo(argv[1]);

  std::cout << "Started creating image directory...\n";
  DWIMG::create_dir(fs::current_path() / "images");
  std::cout << "Finished creating image directory...\n";

  std::cout << "Started removing source images...\n";
  DWIMG::clear_img(fs::current_path() / "images");
  std::cout << "Finished removing source images...\n";

  std::cout << "Started downloading source images...\n";
  DWIMG::download_img(
    "./images/", 100, photo.piece.width, photo.piece.height);
  std::cout << "Finished downloading source images...\n";

  std::cout << "Started calculating average RGB for each source image...\n";
  Photomosaics::load_src();
  std::cout << "Finished calculating...\n";

  std::cout << "Started mosaicifying...\n";
  photo.mosaicify("output.jpg");
  std::cout << "Finished mosaicifying...\n";

  // auto map = photo.get_input_color_map();
  // std::array<struct RGB, SRC> src_map;
  
  // Caching::write_input_json(
  //   (fs::current_path() / "input.json").string(), "webcam.png", map);
  // Caching::write_src_json(
  //   fs::current_path() / DIR / "source.json", Photomosaics::src_color_map);

  // std::vector<std::array<struct RGB, BLOCKS>> color_map;

  // Caching::read_input_json("input.json", "webcam.png", color_map);
  // std::cout << "Size of color_map: " << color_map.size() << "\n";

  // for (size_t i = color_map.size() - 1; i < color_map.size(); i++) {
  //   for (size_t j = 0; j < color_map[0].size(); j++) {
  //     std::cout << "r: " << color_map[i][j].R << ", g: " << color_map[i][j].G << ", b: " << color_map[i][j].B << "\n";
  //   }
  // }
  // Caching::read_src_json(fs::current_path() / DIR / "source.json", src_map);
}