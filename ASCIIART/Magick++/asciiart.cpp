#include <cstdlib>      // atoi
#include <cmath>        // round

#include <iostream>
#include <iomanip>      // setw
#include <string>       // substr
#include <tuple>        // tuple, get
#include <functional>   // function
#include <vector>
#include <set>

#include <Magick++.h>

#include "../terminal-color.hpp"

const std::set<std::string> Color {
  "black", "red", "green",
  "yellow", "blue", "magenta",
  "cyan", "white"
};

const std::set<std::string> Type {
  "fore",
  "back"
};

const std::set<std::string> Style {
  "bold",
  "underline",
  "invisible"
};

struct RGB {
  size_t R;
  size_t G;
  size_t B;
};

void error(const char* msg)
{
  std::cerr << "Usage: " << msg << " FILENAME [-w=WIDTH] [-h=HEIGHT] [BRIGHTNESS] "
            << "[-invt] [--color=COLOR] [--type=TYPE]\n\n"
            << "-w=WIDTH\n"
            << "\tSet the width for scaling.\n"
            << "\tBy default, set to the initial width.\n"
            << "\tWIDTH is integer, which resizes an image.\n"
            << "-h=HEIGHT\n"
            << "\tSet the height for scaling.\n"
            << "\tBy default, set to the initial height.\n"
            << "\tHEIGHT is integer, which resizes an image.\n"
            << "BRIGHTNESS\n"
            << "\tA function that calulates brightness for each pixel.\n"
            << "\tBy default, set to luminosity\n"
            << "\tAlgorithm options:\n"
            << "\t\t- average: (R + G + B) / 3\n"
            << "\t\t- min_max: (max(R, G, B) + min(R, G, B)) / 2\n"
            << "\t\t- luminosity[DEFAULT]: 0.2126R + 0.7152G + 0.0722B\n"
            << "-invt\n"
            << "\tInvert brightness\n"
            << "\tBy default, set to off.\n"
            << "--color=COLOR\n"
            << "\tSet in which color the text is printed.\n"
            << "\tSupported colors:\n"
            << "\t\tblack, red, green, yellow, blue, magenta, cyan, white\n"
            << "--type=TYPE\n"
            << "\tSet in which type the text is printed.\n"
            << "\t\tSupported types:\n"
            << "\t\t\t- fore: foreground\n"
            << "\t\t\t- back: background\n";
}

size_t mapping(size_t n = 0, size_t inputUpperBound = 1, size_t outputUpperBound = 1)
{
  if (inputUpperBound < outputUpperBound)
    return -1;
  return std::round(n * (double) outputUpperBound / inputUpperBound);
}

char* char_to_str(char ch)
{
  char* str = new char[2];
  str[0] = ch;
  str[1] = '\0';

  return str;
}

std::string wrapper(
  const std::string& s,
  const std::string& color,
  const std::string& type,
  const std::string& style
)
{
  std::string coloredString;

  if (color.empty() && type.empty()) {

  } else {
    if (!color.compare("black")) {
      coloredString =
        !type.compare("fore") ? FBLK + s
                              : BBLK + s;
    } else if (!color.compare("red")) {
      coloredString =
        !type.compare("fore") ? FRED + s
                              : BRED + s;
    } else if (!color.compare("green")) {
      coloredString =
        !type.compare("fore") ? FGRN + s
                              : BGRN + s;
      } else if (!color.compare("yellow")) {
      coloredString =
        !type.compare("fore") ? FYEL + s
                              : BYEL + s;
    } else if (!color.compare("blue")) {
      coloredString =
        !type.compare("fore") ? FBLU + s
                              : BBLU + s;
    } else if (!color.compare("magenta")) {
      coloredString =
        !type.compare("fore") ? FMAG + s
                              : BMAG + s;
    } else if (!color.compare("cyan")) {
      coloredString =
        !type.compare("fore") ? FCYN + s
                              : BCYN + s;
    } else if (!color.compare("white")) {
      coloredString =
        !type.compare("fore") ? FWHT + s
                              : BWHT + s;
    }
  }
  
  return coloredString + RST;
};

int main(int argc, char *argv[])
{
  Magick::InitializeMagick(*argv);

  if (argc < 2) {
    error(argv[0]);
    exit(EXIT_FAILURE);
  }

  Magick::Image image;

  try {
    image.read(argv[1]);
  } catch(std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  size_t WIDTH(image.baseColumns());
  size_t HEIGHT(image.baseRows());
  bool invertBrightness = false;
  std::string color = "";
  std::string type = "";
  std::string style = "";

  std::function<uint8_t(size_t, size_t, size_t, bool)> brightness =
    [](size_t R, size_t G, size_t B, bool invert = false) {
      return invert ?
              (uint8_t) (255 - std::round(0.2126 * R + 0.7152 * G + 0.0722 * B))
            : (uint8_t) std::round(0.2126 * R + 0.7152 * G + 0.0722 * B);
    };

  std::vector<std::string> flags;
  if (argc > 2) {
    for (size_t i = 2; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == '-')
          flags.push_back((char*) (argv[i] + 2));
        else
          flags.push_back((char*) (argv[i] + 1));   // remove dash
      } else {
        flags.push_back(argv[i]);
      }
    }

    for (const auto& flag : flags) {
      if (flag[0] == 'w') {
        WIDTH = std::atoi(flag.c_str() + 2);
      } else if (flag[0] == 'h') {
        HEIGHT = std::atoi(flag.c_str() + 2);
      } else if (!flag.compare("invt")) {
        invertBrightness = true;
      } else if (!flag.compare("average")) {
        brightness = [](size_t R, size_t G, size_t B, bool invert = false) {
          return invert ?
                  (uint8_t) (255 - std::round((R + G + B) / 3))
                : (uint8_t) std::round((R + G + B) / 3);
        };
      } else if (!flag.compare("min_max")) {
        brightness = [](size_t R, size_t G, size_t B, bool invert = false) {
          return invert ?
                  (uint8_t) (255 - std::round(
                    (std::max({R, G, B}) + std::min({R, G, B})) / 2))
                : (uint8_t) std::round(
                    (std::max({R, G, B}) + std::min({R, G, B})) / 2);
        };
      } else if (!flag.compare("luminosity")) {
        brightness = [](size_t R, size_t G, size_t B, bool invert = false) {
          return invert ?
                  (uint8_t) (255 - std::round(0.2126 * R + 0.7152 * G + 0.0722 * B))
                : (uint8_t) std::round(0.2126 * R + 0.7152 * G + 0.0722 * B);
        };
      } else if (!flag.substr(0, 5).compare("color")) {
        if (Color.find(flag.substr(6)) != Color.end()) {
          color = flag.substr(6);
        } else {
          error(argv[0]);
          exit(EXIT_FAILURE);
        }
      } else if (!flag.substr(0, 4).compare("type")) {
        if (Type.find(flag.substr(5)) != Type.end()) {
          type = flag.substr(5);
        } else {
          error(argv[0]);
          exit(EXIT_FAILURE);
        }
      } else if (!flag.substr(0, 5).compare("style")) {
        if (Style.find(flag.substr(6)) != Style.end()) {
          style = flag.substr(6);
        } else {
          error(argv[0]);
          exit(EXIT_FAILURE);
        }
      } else {
        error(argv[0]);
        exit(EXIT_FAILURE);
      }
    }
  }
  
  std::string imgPath(image.baseFilename());
  std::string filename(imgPath.substr(imgPath.find('/') + 1, imgPath.length()));
  std::string filenameWOExt(filename.substr(0, filename.find('.')));  

  image.scale(Magick::Geometry(WIDTH, HEIGHT));

  RGB pixels[WIDTH][HEIGHT];

  for (size_t w = 0; w < WIDTH; w++) {
    for (size_t h = 0; h < HEIGHT; h++) {
      pixels[w][h] = {
        (uint8_t) std::round(Magick::Color(image.pixelColor(w, h)).redQuantum() / 256.0),
        (uint8_t) std::round(Magick::Color(image.pixelColor(w, h)).greenQuantum() / 256.0),
        (uint8_t) std::round(Magick::Color(image.pixelColor(w, h)).blueQuantum() / 256.0)
      };
    }
  }

  // for (size_t h = 0; h < HEIGHT; h++) {
  //   for (size_t w = 0; w < WIDTH; w++) {
  //     std::cout << "(" << std::setw(3) << std::get<0>(pixels[w][h]) << ", "
  //               << std::setw(3) << std::get<1>(pixels[w][h]) << ", "
  //               << std::setw(3) << std::get<2>(pixels[w][h]) << ") ";
  //   }
  //   std::cout << '\n';
  // }

  unsigned brightnesses[WIDTH][HEIGHT];
  
  for (size_t w = 0; w < WIDTH; w++) {
    for (size_t h = 0; h < HEIGHT; h++) {
      brightnesses[w][h] = brightness(
        pixels[w][h].R,
        pixels[w][h].G,
        pixels[w][h].B,
        invertBrightness
      );
    }
  }

  // for (size_t h = 0; h < HEIGHT; h++) {
  //   for (size_t w = 0; w < WIDTH; w++) {
  //     std::cout << std::setw(3) << brightnesses[w][h] << " ";
  //   }
  //   std::cout << '\n';
  // }

  const std::string ascii(" `^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$");

  char asciiMatrix[WIDTH][HEIGHT];

  for (size_t w = 0; w < WIDTH; w++) {
    for (size_t h = 0; h < HEIGHT; h++) {
      asciiMatrix[w][h] = ascii[mapping(brightnesses[w][h], 255, ascii.length() - 1)];
    }
  }

  for (size_t h = 0; h < HEIGHT; h++) {
    for (size_t w = 0; w < WIDTH; w++) {
      std::cout << wrapper(char_to_str(asciiMatrix[w][h]), color, type, style);
    }
    std::cout << '\n';
  }

  return 0;
}