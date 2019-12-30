#include "ImageMagick-7/Magick++.h"

#include "Photomosaics.hpp"

int main(int argc, char* argv[])
{
  Magick::InitializeMagick(*argv);

  Magick::Image image;
  image.read("ff_x20_008.JPG");
  auto avg_color = Photomosaics::calc_avg_color(image);
  std::cout << "Avg R: " << avg_color.R << "\n"
            << "Avg G: " << avg_color.G << "\n"
            << "Avg B: " << avg_color.B << "\n";
}