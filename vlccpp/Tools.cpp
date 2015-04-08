#include "stdafx.h"
#include "Tools.h"

namespace vlc {
  std::map<std::string, cv::Mat> Tools::Images;
  std::map<std::string, std::vector<std::string>> Tools::Messages;
  std::map<std::string, int> Tools::Windows;
  int Tools::WindowsNum=0;

  void Tools::PrintMessage(std::string WindowName, std::string Message)
  {
    if (Messages.find(WindowName) == Messages.end()) {
      Messages[WindowName] = std::vector<std::string>();
    }
    Messages[WindowName].push_back(Message);
    RefreshWindow(WindowName);
  }
  void Tools::ShowImage(std::string WindowName, cv::Mat Image)
  {
    Images[WindowName] = cv::Mat(Image);
    RefreshWindow(WindowName);
  }
  void Tools::RefreshWindow(std::string WindowName)
  {
    int i;
    if (Windows.find(WindowName) == Windows.end())
      Windows[WindowName] = WindowsNum++;
    cv::Mat ImageForDisplay;
    if (Images.find(WindowName) != Images.end()) {
      cv::resize(Images[WindowName], ImageForDisplay, cv::Size(300, 300));
    }
    else {
      ImageForDisplay = cv::Mat(300, 300, CV_8UC1, cv::Scalar(0, 0, 0));
    }

    if (Messages.find(WindowName) != Messages.end()) {
      i = 0;
      for (std::vector<std::string>::const_iterator iter = Messages[WindowName].begin(); iter != Messages[WindowName].end(); ++iter) {
        cv::putText(ImageForDisplay, *iter, cv::Point(0, (++i) * 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
        printf("[%s]:%s\n", WindowName.c_str(), iter->c_str());
      }
    }

    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(WindowName,  Windows[WindowName]*300,0);
    cv::imshow(WindowName, ImageForDisplay);
  }
  Tools::Tools()
  {
  }


  Tools::~Tools()
  {
  }
}
