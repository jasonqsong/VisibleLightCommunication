#pragma once
#include "stdafx.h"
namespace vlc {
  class Processor
  {
  public:
    static std::vector<vlc::Light>* OpenFFT(cv::Mat image, CameraInfo* camera);
    static void PairLights(std::vector<vlc::Light>* Lights, RoomInfo* Room);
    static void AOA(std::vector<vlc::Light>* Lights, CameraInfo* Camera);
    Processor();
    virtual ~Processor();
  };

}
