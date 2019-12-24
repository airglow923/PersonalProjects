#include "Photomosaics.hpp"

Photomosaics::Photomosaics(
  std::string filename_, unsigned width_, unsigned height_)
  : filename(filename_), img_no(0), width(width_), height(height_)
{
}

void Photomosaics::download_src_imgs()
{
  // random variables
  std::random_device dev;
  std::mt19937 seed(dev());
  std::uniform_int_distribution<std::mt19937::result_type> range;

  system(("mkdir " + DIR + filename).c_str());

  for (int i = 0; i < 10; i++) {
    download_src_img(range(seed), 200, 100);
    img_no++;
  }
}

void Photomosaics::download_src_img(
  std::mt19937::result_type seed, unsigned w, unsigned h)
{
  CURL* curl;
  CURLcode curl_result;
  FILE* fp = nullptr;
  const char* url = ("https://picsum.photos/seed/" + std::to_string(seed) + "/" + std::to_string(w) + "/" + std::to_string(h)).c_str(); 
  curl = curl_easy_init();

  std::cout << seed << "\n";

  if (curl) {
    fp = fopen((DIR + filename + "/" + std::to_string(img_no) + ".jpg").c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_result = curl_easy_perform(curl);
    if (curl_result)
      std::cout << "Cannot find the image\n";
  }
  
  // curl_easy_cleanup(curl);
  fclose(fp);
}

void Photomosaics::clear_src_img()
{
  system(("rm " + fs::current_path().string() + filename + "/*").c_str());
  img_no = 0;
}