#pragma once
#include "stdafx.h"
namespace vlc {
  class Tools
  {
  public:
    static std::map<std::string, cv::Mat> Images;
    static std::map<std::string, std::vector<std::string>> Messages;
    static std::map<std::string, int> Windows;
    static int WindowsNum;
    static void PrintMessage(std::string WindowName, std::string Message);
    static void ShowImage(std::string WindowName, cv::Mat Image);
    static void Plot(std::string WindowName, cv::Mat Data,cv::Scalar Color);
    static void RefreshWindow(std::string WindowName);
    static void ReduceRowByMost(cv::InputArray _src, cv::OutputArray _dst, cv::InputArray _mask);
    static unsigned int ConvertFromBinToDec(std::string BinStr);
    Tools();
    virtual ~Tools();
  };

}
