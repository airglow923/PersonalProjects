#include <iostream>

#include <opencv2/opencv.hpp>

int main()
{
  cv::VideoCapture cam;

  if (!cam.open(0))
    return 1;

  while (true) {
    cv::Mat frame;

    cam >> frame;
    if (frame.empty())
      break;
    cv::imshow("", frame);

    switch (cv::waitKey(1)) {
    case 10:        // carriage return to release
      cv::imwrite("webcam.png", frame);
      goto BREAK;
    case 27:        // esc to close
      goto BREAK;
    }
  }

BREAK:
  return 0;
}