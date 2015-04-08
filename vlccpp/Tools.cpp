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
    Images[WindowName] = Image.clone();
    RefreshWindow(WindowName);
  }
  void Tools::Plot(std::string WindowName, cv::Mat Data,cv::Scalar Color)
  {
    cv::Mat Image;
    if (Images.find(WindowName) == Images.end()) {
      Images[WindowName]= cv::Mat(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));
    }
    Image = Images[WindowName];
    cv::vector<cv::Point> Points;
    for (int i = 0; i < Data.size[1]; i++) {
      if (Data.type() == CV_32FC1)
        Points.push_back(cv::Point(300*i/Data.size[1],Image.size[0]/2-Data.at<float>(0, i)/Image.size[0]/2));
    }
    cv::polylines(Image, Points, false, Color, 1);
    Images[WindowName]=Image.clone() ;
    RefreshWindow(WindowName);
  }
  void Tools::RefreshWindow(std::string WindowName)
  {
    if (Windows.find(WindowName) == Windows.end())
      Windows[WindowName] = WindowsNum++;
    cv::Mat ImageForDisplay;
    if (Images.find(WindowName) == Images.end()) {
      Images[WindowName]=cv::Mat(800, 800, CV_8UC1, cv::Scalar(0, 0, 0));
    }
    cv::resize(Images[WindowName], ImageForDisplay, cv::Size(800, 800));

    if (Messages.find(WindowName) != Messages.end()) {
      for (int i = 0;i< Messages[WindowName].size(); ++i) {
        cv::putText(ImageForDisplay, Messages[WindowName].at(i), cv::Point(0, (i+1) * 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
        printf("[%s]:%s\n", WindowName.c_str(), Messages[WindowName].at(i).c_str());
      }
    }

    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(WindowName,  (Windows[WindowName]%6)*300,(Windows[WindowName]/6)*300);
    cv::imshow(WindowName, ImageForDisplay);
    cv::imwrite(WindowName + ".jpg", Images[WindowName]);
  }
  Tools::Tools()
  {
  }


  Tools::~Tools()
  {
  }
}
