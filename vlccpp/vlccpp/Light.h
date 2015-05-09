#pragma once
#include "stdafx.h"
namespace vlc {
  class Light
  {
  public:
    cv::Point2d PositionInImage;
    cv::Point3d PositionInRoom;
    double Frequency;
    Light();
    virtual ~Light();
  };
}

