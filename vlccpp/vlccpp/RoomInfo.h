#pragma once
#include "stdafx.h"
namespace vlc {

  class RoomInfo
  {
  public:

    std::map<double, cv::Point3d> Transmitters;
    RoomInfo();
    virtual ~RoomInfo();
  };

}
