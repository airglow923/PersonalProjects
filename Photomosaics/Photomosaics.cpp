#include "Photomosaics.hpp"

Photomosaics::Photomosaics(const std::string& filename_)
  : img_no(0)
{
  load_img(filename_);
  color_map.reserve(BLOCKS);
  block_map.reserve(BLOCKS);
}

void Photomosaics::adjust_piece()
{
  piece.width   = (unsigned) (width / BLOCKS);
  piece.height  = (unsigned) (height / BLOCKS);
}

void Photomosaics::load_img(const std::string& s)
{
  Magick::Image image;

  try {
    image.read(s);

    filename  = s;
    width     = (size_t) image.columns();
    height    = (size_t) image.rows();
    
    adjust_piece();
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
}

struct RGB Photomosaics::calc_avg_color(
  Magick::Image image,  // local copy does not alter the original object
  unsigned width, unsigned height,
  unsigned xOff, unsigned yOff
)
{
  struct RGB avg_color;
  unsigned total_pixels = width * height;
  double total_R = 0;
  double total_G = 0;
  double total_B = 0;

  image.crop(Magick::Geometry(width, height, xOff, yOff));

  for (size_t w = xOff; w < xOff + width; w++) {
    for (size_t h = yOff; h < yOff + height; h++) {
      // Since ImageMagick-7, quantumRed is used instead of redQuamtum
      total_R += image.pixelColor(w, h).quantumRed() / 256.0;
      total_G += image.pixelColor(w, h).quantumGreen() / 256.0;
      total_B += image.pixelColor(w, h).quantumBlue() / 256.0;
    }
  }

  avg_color = {
    (unsigned) round(total_R / total_pixels),
    (unsigned) round(total_G / total_pixels),
    (unsigned) round(total_B / total_pixels)
  };

  return avg_color;
}

struct RGB Photomosaics::calc_avg_color(const std::string& s)
{
  Magick::Image image;

  try {
    image.read(s);
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    exit(1);
  }

  return calc_avg_color(image, image.columns(), image.rows(), 0, 0);
}

void Photomosaics::pixellate()
{
  Magick::Image image;
  int i;

  try {
    image.read(filename);
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    return;
  }

  for (size_t xOff = 0; xOff < width; xOff += piece.width) {
    std::array<struct RGB, BLOCKS> width_color_map;
    i = 0;
    for (size_t yOff = 0; yOff < height; yOff += piece.height) {
      unsigned w =
        (xOff != width && xOff + piece.width > width) ?
          width - xOff : piece.width;
      unsigned h =
        (yOff != height && yOff + piece.height > height) ?
          height - yOff : piece.height;
      width_color_map[i++] = calc_avg_color(image, w, h, xOff, yOff);
    }
    color_map.push_back(width_color_map);
  }
}

void Photomosaics::load_src()
{
  fs::path path(fs::current_path() / DIR);

  for (auto& p : fs::directory_iterator(fs::absolute(path)))
    if (!p.is_directory() && !p.path().extension().compare(".jpg"))
      src_color_map[atoi(p.path().stem().c_str())] = calc_avg_color(p.path());
}

double Photomosaics::calc_color_difference(
  const struct RGB& rgb1, const struct RGB& rgb2
)
{
  return sqrt(
    pow(rgb2.R - rgb1.R, 2.0) +
    pow(rgb2.G - rgb1.G, 2.0) +
    pow(rgb2.B - rgb1.B, 2.0)
  );
}

void Photomosaics::build_block_map()
{
  for (size_t col = 0; col < BLOCKS; col++) {
    std::array<unsigned, BLOCKS> positions;
    int j = 0;
    for (size_t row = 0; row < BLOCKS; row++) {
      double smallest =
        calc_color_difference(color_map[col][row], src_color_map[0]);
      unsigned pos = 0;
      for (size_t i = 1; i < src_color_map.size(); i++) {
        double val =
          calc_color_difference(color_map[col][row], src_color_map[i]);
        if (smallest > val) {
          smallest = val;
          pos = i;
        }
      }
      positions[j++] = pos;
    }
    block_map.push_back(positions);
  }
}

void Photomosaics::mosaicify(const std::string& s)
{
  std::cout << "Started building block map...\n";
  build_block_map();
  std::cout << "Finished building block map...\n";

  Magick::Image image;

  try {
    image.read(filename);
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    exit(1);
  }

  for (size_t xOff = 0, col = 0; xOff < width; xOff += piece.width, col++) {
    for (size_t yOff = 0, row = 0; yOff < height; yOff += piece.height, row++) {
      unsigned w =
        (xOff != width && xOff + piece.width > width) ?
          width - xOff : piece.width;
      unsigned h =
        (yOff != height && yOff + piece.height > height) ?
          height - yOff : piece.height;
      
      Magick::Image src_img;

      try {
        src_img.read(fs::current_path() / DIR / (std::to_string(block_map[col][row]) + ".jpg"));
      } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        exit(1);
      }

      src_img.scale(Magick::Geometry(w, h));

      image.composite(src_img, xOff, yOff);
    }
  }

  image.write(s);
}

void Photomosaics::disp_color_map()
{
  pixellate();
  std::cout << "Successfully pixellate the input image\n";
  for (int h = 0; h < 10; h++) {
    for (int w = 0; w < 10; w++) {
      std::cout << "(" << std::setw(3) << color_map[w][h].R << " "
                << std::setw(3) << color_map[w][h].G << " "
                << std::setw(3) << color_map[w][h].B << ") ";
    }
    std::cout << "\n";
  }
}