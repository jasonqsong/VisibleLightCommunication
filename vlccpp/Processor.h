#pragma once
#include "stdafx.h"
namespace vlc {
  class Processor
  {
  public:
    static void OpenFFT(cv::Mat image, CameraInfo* camera);
    Processor();
    virtual ~Processor();
  };

}
