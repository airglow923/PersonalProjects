#include "Photomosaics.hpp"

unsigned Photomosaics::src = 0;

Photomosaics::Photomosaics(const std::string& filename_)
{
  load_img(filename_);
  adjust_piece();
  create_src_dir();
}

Photomosaics::Photomosaics(
  const std::string& filename_, unsigned width_, unsigned height_)
  : filename(filename_), img_no(0), width(width_), height(height_)
{
  adjust_piece();
  create_src_dir();
}

void Photomosaics::load_img(const std::string& s)
{
  Magick::Image image;
  
  try {
    image.read(s);
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    return;
  }
  
  filename = s;
  width = (size_t) image.baseColumns();
  height = (size_t) image.baseRows();
  
  adjust_piece();
}

void Photomosaics::adjust_piece()
{
  piece.width = width / BLOCKS;
  piece.height = height / BLOCKS;
}

struct RGB Photomosaics::calc_avg_color(const Magick::Image& image)
{
  const unsigned WIDTH = image.baseColumns();
  const unsigned HEIGHT = image.baseRows();
  
  struct RGB avg_color;
  unsigned total_pixels = WIDTH * HEIGHT;
  unsigned total_R = 0;
  unsigned total_G = 0;
  unsigned total_B = 0;

  for (size_t w = 0; w < WIDTH; w++) {
    for (size_t h = 0; h < HEIGHT; h++) {
      total_R += (uint8_t) round(image.pixelColor(w, h).redQuantum() / 256.0);
      total_G += (uint8_t) round(image.pixelColor(w, h).greenQuantum() / 256.0);
      total_R += (uint8_t) round(image.pixelColor(w, h).blueQuantum() / 256.0);
    }
  }

  avg_color = {
    total_R / total_pixels,
    total_G / total_pixels,
    total_B / total_pixels
  };

  return avg_color;
}

void Photomosaics::mosaicify()
{
  Magick::Image image;

  try {
    image.read(filename);
  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
    return;
  }

  for (unsigned w = 0; w < width; w += piece.width) {
    Magick::Image tmp(image);
    std::vector<struct RGB> width_color_map;
    for (unsigned h = 0; h < height; h += piece.height) {
      if (w > width || h > height)
        tmp.crop(Magick::Geometry(width - w, height - h, w, h));
      else
        tmp.crop(Magick::Geometry(piece.width, piece.height, w, h));
      width_color_map.push_back(calc_avg_color(tmp));
    }
    color_map.push_back(width_color_map);
  }
}

double Photomosaics::calc_color_difference(
  const struct RGB& rgb1, const struct RGB& rgb2
)
{
  return sqrt(
    pow(rgb2.R - rgb1.R, (double) 2) +
    pow(rgb2.G - rgb1.G, (double) 2) +
    pow(rgb2.B - rgb1.B, (double) 2)
  );
}

static size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
  FILE* stream = (FILE*) userp;

  return !stream ? 0 : fwrite((FILE*) buffer, size, nmemb, stream);
}

static void add_transfer(CURLM* multi_handle, std::string url, FILE* fp)
{
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_multi_add_handle(multi_handle, curl);
}

void Photomosaics::download_sprt_src_img()
{
  // random variables
  std::random_device dev;
  std::mt19937 seed(dev());
  std::uniform_int_distribution<std::mt19937::result_type> range;

  create_src_dir();

  CURLM* multi_handle = curl_multi_init();
  CURLMsg* msg;
  FILE* files[SRC];

  int still_alive = 0;
  int msgs_left = -1;

  for (int i = 0; i < SRC; i++) {
    std::string url = "https://picsum.photos/seed/" + std::to_string(range(seed)) + "/" + std::to_string(piece.width) + "/" + std::to_string(piece.height);
    files[i] = fopen((DIR + filename + "/" + std::to_string(img_no++) + ".jpg").c_str(), "w");
    add_transfer(multi_handle, url, files[i]);
  }

  do {
    curl_multi_perform(multi_handle, &still_alive);
    while ((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
      if (msg->msg == CURLMSG_DONE) {
        CURL* curl = msg->easy_handle;
        curl_multi_remove_handle(multi_handle, curl);
        curl_easy_cleanup(curl);
      }
    }
    if (still_alive)
      curl_multi_wait(multi_handle, NULL, 0, 1000, NULL);
  } while (still_alive);

  curl_multi_cleanup(multi_handle);
  
  for (int i = 0; i < SRC; i++)
    fclose(files[i]);
}

void Photomosaics::clear_sprt_src_img()
{
  system(("rm " + fs::current_path().string() + filename + "/*").c_str());
  img_no = 0;
}

void Photomosaics::create_src_dir()
{
  system(("bash check_dir.sh " + filename).c_str());
}

void Photomosaics::download_src_img(unsigned w, unsigned h)
{
  // random variables
  std::random_device dev;
  std::mt19937 seed(dev());
  std::uniform_int_distribution<std::mt19937::result_type> range;

  CURLM* multi_handle = curl_multi_init();
  CURLMsg* msg;
  FILE* files[SRC];

  int still_alive = 0;
  int msgs_left = -1;

  for (int i = 0; i < SRC; i++) {
    std::string url = "https://picsum.photos/seed/" + std::to_string(range(seed)) + "/" + std::to_string(w) + "/" + std::to_string(h);
    std::cout << url << "\n";
    files[i] = fopen((DIR + std::to_string(src++) + ".jpg").c_str(), "w");
    add_transfer(multi_handle, url, files[i]);
  }

  do {
    curl_multi_perform(multi_handle, &still_alive);
    while ((msg = curl_multi_info_read(multi_handle, &msgs_left))) {
      if (msg->msg == CURLMSG_DONE) {
        CURL* curl = msg->easy_handle;
        curl_multi_remove_handle(multi_handle, curl);
        curl_easy_cleanup(curl);
      }
    }
    if (still_alive)
      curl_multi_wait(multi_handle, NULL, 0, 1000, NULL);
  } while (still_alive);

  curl_multi_cleanup(multi_handle);
  
  for (int i = 0; i < SRC; i++)
    fclose(files[i]);
}

void Photomosaics::disp_color_map()
{
  mosaicify();
  for (int w = 0; w < BLOCKS; w++) {
    for (int h = 0; h < BLOCKS; h++) {
      std::cout << std::setw(3) << color_map[w][h].R << "\n"
                << std::setw(3) << color_map[w][h].G << "\n"
                << std::setw(3) << color_map[w][h].B << "\n";
    }
    std::cout << "\n";
  }
}