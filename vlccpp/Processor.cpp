#include "stdafx.h"
#include "Processor.h"

namespace vlc {

  void Processor::OpenFFT(cv::Mat ARGBImage,CameraInfo* Camera) {

    cv::Mat GrayImage;
    cv::cvtColor(ARGBImage, GrayImage, CV_RGB2GRAY);

  }

  Processor::Processor()
  {
  }


  Processor::~Processor()
  {
  }
}
