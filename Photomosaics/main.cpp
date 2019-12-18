#include "Photomosaics.hpp"

int main()
{
  CURL* image;
  CURLcode curl_result;
  FILE* fp = nullptr;
  const char* url = "https://picsum.photos/400";
  image = curl_easy_init();

  if (image) {
    fp = fopen("400.jpg", "w");
    if (fp == NULL)
      std::cout << "File cannot be found.\n";
    curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(image, CURLOPT_URL, url);
    curl_result = curl_easy_perform(image);
    if (curl_result)
      std::cout << "Cannot find an image\n";
  }
  curl_easy_cleanup(image);
  fclose(fp);
  return 0;
}