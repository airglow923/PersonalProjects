#include "Photomosaics.hpp"

int main(int argc, char* argv[])
{
  Magick::InitializeMagick(*argv);
  Photomosaics a("sample.JPG");
  a.disp_color_map();
}