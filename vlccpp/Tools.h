#pragma once
#include "stdafx.h"
namespace vlc {
  class Tools
  {
  public:
    static std::map<std::string, cv::Mat> Images;
    static std::map<std::string, std::vector<std::string>> Messages;
    static void PrintMessage(std::string WindowName, std::string Message);
    static void ShowImage(std::string WindowName, cv::Mat Image);
    static void RefreshWindow(std::string WindowName);
    Tools();
    virtual ~Tools();
  };

}
