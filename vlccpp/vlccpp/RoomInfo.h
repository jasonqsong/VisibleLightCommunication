#pragma once
#include "stdafx.h"
namespace vlc {

  class RoomInfo
  {
  public:

    std::map<unsigned int, cv::Point3d> Transmitters;
    RoomInfo();
    virtual ~RoomInfo();
  };

}
