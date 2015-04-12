#pragma once
#include "stdafx.h"
namespace vlc {
  class Processor
  {
  public:
    static std::vector<vlc::Transmitter>* OpenFFT(cv::Mat image, CameraInfo* camera);
    static void PairLights(std::vector<vlc::Transmitter>* Lights, RoomInfo* Room);
    static void AOA(std::vector<vlc::Transmitter>* Lights, CameraInfo* Camera);
    Processor();
    virtual ~Processor();
  };

}
