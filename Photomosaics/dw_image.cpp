#include "dw_image.hpp"

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

void DWIMG::download_img(
  const std::string& path,
  int num,
  unsigned width,
  unsigned height
)
{
  if (!num || !width || !height)
    return;

  // random variables
  std::random_device dev;
  std::mt19937 seed(dev());
  std::uniform_int_distribution<std::mt19937::result_type> range;

  CURLM* multi_handle = curl_multi_init();
  CURLMsg* msg;
  FILE* files[num];

  int still_alive = 0;
  int msgs_left = -1;

  for (int i = 0; i < num; i++) {
    std::string url = "https://picsum.photos/seed/" + std::to_string(range(seed)) + "/" + std::to_string(width) + "/" + std::to_string(height);
    std::cout << url << "\n";
    files[i] = fopen((path + std::to_string(i) + ".jpg").c_str(), "w");
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
  
  for (int i = 0; i < num; i++)
    fclose(files[i]);
}

void DWIMG::clear_img(const std::string& path)
{
  fs::path p(path);
  fs::remove_all(p);
  // system(("rm " + fs::current_path().string() + path + "/*").c_str());
}

void DWIMG::create_dir(const std::string& path)
{
  fs::path p(path);
  fs::directory_entry e(p);
  if (!e.exists())
    fs::create_directory(e);
  // system(("bash check_dir.sh " + path).c_str());
}