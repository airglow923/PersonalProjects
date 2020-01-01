#include <iostream>
#include <iomanip>

#include "Photomosaics.hpp"
#include "dw_image.hpp"

std::array<struct RGB, SRC> Photomosaics::src_color_map;

int main(int argc, char* argv[])
{
  Photomosaics photo("sample.jpg");

  std::cout << "Started downloading source images...\n";
  DWIMG::download_img("./images/", 100, photo.piece.width, photo.piece.height);
  std::cout << "Finished downloading source images...\n";

  std::cout << "Started calculating average RGB for each source image...\n";
  Photomosaics::load_src();
  std::cout << "Finished calculating...\n";

  std::cout << "Started mosaicifying...\n";
  photo.mosaicify("output.jpg");
  std::cout << "Finished mosaicifying...\n";
}