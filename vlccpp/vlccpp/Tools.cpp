#include "stdafx.h"
#include "Tools.h"

namespace vlc {
  std::map<std::string, cv::Mat> Tools::Images;
  std::map<std::string, std::vector<std::string>> Tools::Messages;
  std::map<std::string, int> Tools::Windows;
  int Tools::WindowsNum = 0;

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
  void Tools::Plot(std::string WindowName, cv::Mat Data, cv::Scalar Color)
  {
    cv::Mat Image;
    if (Images.find(WindowName) == Images.end()) {
      Images[WindowName] = cv::Mat(300, 300, CV_8UC3, cv::Scalar(0, 0, 0));
    }
    Image = Images[WindowName];
    cv::vector<cv::Point> Points;
    for (int i = 0; i < Data.size[1]; i++) {
      if (Data.type() == CV_32FC1)
        Points.push_back(cv::Point(300 * i / Data.size[1], Image.size[0] / 2 - Data.at<float>(0, i) / Image.size[0] / 2));
    }
    cv::polylines(Image, Points, false, Color, 1);
    Images[WindowName] = Image.clone();
    RefreshWindow(WindowName);
  }
  void Tools::RefreshWindow(std::string WindowName)
  {
    if (Windows.find(WindowName) == Windows.end())
      Windows[WindowName] = WindowsNum++;
    cv::Mat ImageForDisplay;
    if (Images.find(WindowName) == Images.end()) {
      Images[WindowName] = cv::Mat(300, 300, CV_8UC1, cv::Scalar(0, 0, 0));
    }
    cv::resize(Images[WindowName], ImageForDisplay, cv::Size(300, 300));

    if (Messages.find(WindowName) != Messages.end()) {
      for (unsigned i = 0;i < Messages[WindowName].size(); ++i) {
        cv::putText(ImageForDisplay, Messages[WindowName].at(i), cv::Point(0, (i + 1) * 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
        printf("[%s]:%s\n", WindowName.c_str(), Messages[WindowName].at(i).c_str());
      }
    }

    cv::namedWindow(WindowName, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(WindowName, (Windows[WindowName] % 6) * 300, (Windows[WindowName] / 6) * 300);
    cv::imshow(WindowName, ImageForDisplay);
    cv::imwrite(WindowName + ".jpg", Images[WindowName]);
  }

  void Tools::ReduceRowByMost(cv::InputArray _src, cv::OutputArray _dst, cv::InputArray _mask) {
    cv::Mat src = _src.getMat();
    _dst.create(1, src.cols, CV_8UC1);
    cv::Mat dst = _dst.getMat();
    cv::Mat mask = _mask.getMat();
    if (src.depth() != CV_8U || mask.depth() != CV_8U) {
      throw "TYPE DON'T SUPPORT";
    }
    int i, j, addr;
    cv::Size size = src.size();
    uchar *srcd = src.data;
    uchar *dstd = dst.data;
    uchar *maskd = mask.data;
    std::map<uchar, int> m;
    std::map<uchar, int>::iterator p;
    uchar mostVal;
    uchar rVal;
    int r;
    for (i = 0;i < size.width;++i) {
      m.clear();
      mostVal = 0;
      for (j = 0;j < size.height;++j) {
        addr = j*size.width + i;
        if (!maskd[addr])
          continue;
        for (r = -2;r <= 2;++r) {
          rVal = (uchar)(((int)srcd[addr] + r) % 180);
          if (m.find(rVal) != m.end()) {
            m[rVal]++;
          }
          else {
            m[rVal] = 1;
          }
        }
      }
      if (m.size() == 0) {
        dstd[i] = 0;
        continue;
      }
      mostVal = m.begin()->first;
      for (p = m.begin();p != m.end();++p) {
        if (p->second > m[mostVal]) {
          mostVal = p->first;
        }
      }
      dstd[i] = mostVal;
    }
  }
  unsigned int Tools::ConvertFromBinToDec(std::string BinStr) {
    unsigned int x = 0;
    for (unsigned int i = 0;i<BinStr.size() ;++i) {
      x <<= 1;
      x|=BinStr[i] - '0';
    }
    return x;
  }

  Tools::Tools()
  {
  }


  Tools::~Tools()
  {
  }
}
