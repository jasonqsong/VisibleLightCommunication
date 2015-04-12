#pragma once
#include "stdafx.h"
namespace vlc {
  class Transmitter

  {
  public:
    cv::Point2d PositionInImage;
    cv::Point3d PositionInRoom;
    double Frequency;
    Transmitter();
    virtual ~Transmitter();
  };

}
