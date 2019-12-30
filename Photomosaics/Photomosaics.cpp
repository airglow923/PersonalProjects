#include "Photomosaics.hpp"

Photomosaics::Photomosaics(
  std::string filename_, unsigned width_, unsigned height_)
  : filename(filename_), img_no(0), width(width_), height(height_)
{
  adjust_src();
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
  
  adjust_src();
}

void Photomosaics::adjust_src()
{
  src.width = width / SRC;
  src.height = height / SRC;
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

  create_dir();

  CURLM* multi_handle = curl_multi_init();
  CURLMsg* msg;
  FILE* files[SRC];

  int still_alive = 0;
  int msgs_left = -1;

  for (int i = 0; i < SRC; i++) {
    std::string url = "https://picsum.photos/seed/" + std::to_string(range(seed)) + "/" + std::to_string(src.width) + "/" + std::to_string(src.height);
    std::cout << url << "\n";
    files[i] = fopen((DIR + filename + "/" + std::to_string(img_no) + ".jpg").c_str(), "w");
    img_no++;
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

void Photomosaics::download_src_img()
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
    std::string url = "https://picsum.photos/seed/" + std::to_string(range(seed)) + "/" + std::to_string(src.width) + "/" + std::to_string(src.height);
    std::cout << url << "\n";
    files[i] = fopen((DIR + std::to_string(src) + ".jpg").c_str(), "w");
    src++;
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