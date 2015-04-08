#include "stdafx.h"
#include "Processor.h"

namespace vlc {

  void Processor::OpenFFT(cv::Mat Image, CameraInfo* Camera) {
    //Rotate
    cv::Mat RotateImage;
    if (Image.size[0] < Image.size[1]) {
      double width = Image.size[0] < Image.size[1] ? Image.size[0] : Image.size[1];//get the smaller side
      cv::Point ImageCenter = cv::Point(width*0.5, width *0.5);
      cv::Mat RotationMat = cv::getRotationMatrix2D(ImageCenter, 90, 1);
      cv::warpAffine(Image, RotateImage, RotationMat, cv::Size(Image.size[0], Image.size[1]));
    }
    else {
      RotateImage = cv::Mat(Image);
    }
    vlc::Tools::ShowImage("RotateImage", RotateImage);
    Image = RotateImage;

    //Gray
    cv::Mat GrayImage;
    cv::cvtColor(Image, GrayImage, CV_RGB2GRAY);
    vlc::Tools::ShowImage("GrayImage", GrayImage);
    Image = GrayImage;

    //Blur
    cv::Mat BlurImage;
    cv::blur(Image, BlurImage, cv::Size(50, 50));
    vlc::Tools::ShowImage("BlurImage", BlurImage);
    Image = BlurImage;

    //Threshold
    cv::Mat ThresholdImage;
    cv::threshold(Image, ThresholdImage, 0, 255, cv::THRESH_OTSU);
    vlc::Tools::ShowImage("ThresholdImage", ThresholdImage);
    Image = ThresholdImage;
    if (Image.type() != CV_8UC1)
      vlc::Tools::PrintMessage("ThresholdImage","Not CV_8UC1");
    else
      vlc::Tools::PrintMessage("ThresholdImage","Is CV_8UC1");

    cv::Mat ContoursImage;
    std::vector<std::vector<cv::Point>> Contours;
    std::vector<cv::Vec4i> Hierarchy;
    cv::findContours(Image, Contours, Hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE,cv::Point(0,0));
    //ContoursImage.create(Image.size(), CV_8UC3);
    //for (int i = 0; i < Contours.size(); ++i)
    //  cv::drawContours(ContoursImage, Contours, i, cv::Scalar(0, 0, 0), 1, 8, Hierarchy);
    //vlc::Tools::ShowImage("ContoursImage", ContoursImage);
    Image = ContoursImage;


  }

  Processor::Processor()
  {
  }


  Processor::~Processor()
  {
  }
}
