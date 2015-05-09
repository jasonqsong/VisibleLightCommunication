#pragma once
namespace vlc {
  class CameraInfo
  {
  public:
    double RollingShutterRate;
    double Focal;
    CameraInfo();
    virtual ~CameraInfo();
  };


}