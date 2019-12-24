#include <iostream>
#include <string>

#include "curl/curl.h"

int main()
{
	CURL* image;
	CURLcode curl_result;
	FILE* fp = nullptr;
	image = curl_easy_init();
	
	const char* url = "https://picsum.photos/seed/798787987897/200/100";
	if (image) {
		fp = fopen("0.jpg", "w");
		curl_easy_setopt(image, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(image, CURLOPT_URL, url);
		curl_result = curl_easy_perform(image);
		if (curl_result)
			std::cout << "Cannot find an image\n";
	}
	fclose(fp);

	url = "https://picsum.photos/seed/879879872394/200/100";
	if (image) {
		fp = fopen("1.jpg", "w");
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
}
