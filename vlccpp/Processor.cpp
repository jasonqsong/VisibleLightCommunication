#include "stdafx.h"
#include "Processor.h"

namespace vlc {

  void Processor::OpenFFT(cv::Mat Image,CameraInfo* Camera) {

    cv::Mat GrayImage;
    cv::cvtColor(Image, GrayImage, CV_RGB2GRAY);
    vlc::Tools::ShowImage("GrayImage", GrayImage);


  }

  Processor::Processor()
  {
  }


  Processor::~Processor()
  {
  }
}
