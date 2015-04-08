#include "stdafx.h"
#include "Processor.h"

namespace vlc {

  void Processor::OpenFFT(cv::Mat Image, CameraInfo* Camera) {
    //Rotate
    cv::Mat RotateImage;
    if (Image.size[0] < Image.size[1]) {
      double width = Image.size[0] < Image.size[1] ? Image.size[0] : Image.size[1];//get the smaller side
      cv::Point2d ImageCenter = cv::Point2d(width*0.5, width *0.5);
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
    vlc::Tools::PrintMessage("GrayImage", std::to_string(GrayImage.size[0]));
    vlc::Tools::PrintMessage("GrayImage", std::to_string(GrayImage.size[1]));
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

    //Contours
    cv::Mat ContoursImage = GrayImage.clone();
    cv::vector<cv::vector<cv::Point>>* Contours = new std::vector<std::vector<cv::Point>>();
    cv::findContours(Image, *Contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    for (unsigned i = 0; i < Contours->size(); ++i)
      cv::drawContours(ContoursImage, *Contours, i, cv::Scalar(255, 255, 255), 5);
    vlc::Tools::ShowImage("ContoursImage", ContoursImage);

    //EnclosingCircle
    cv::Mat EnclosingCircleImage = GrayImage.clone();
    cv::vector<cv::Point2f>* Centers = new cv::vector<cv::Point2f>(Contours->size());
    cv::vector<float>* Radiuses = new cv::vector<float>(Contours->size());
    for (unsigned i = 0; i < Contours->size(); ++i)
      cv::minEnclosingCircle(((*Contours)[i]), (*Centers)[i], (*Radiuses)[i]);
    for (unsigned i = 0; i < Contours->size(); ++i)
      cv::circle(EnclosingCircleImage, (*Centers)[i], (int)(*Radiuses)[i], cv::Scalar(255, 255, 255), 5);
    vlc::Tools::ShowImage("EnclosingCircle", EnclosingCircleImage);

    cv::Mat SliceImage = GrayImage.clone();
    double Fs = 1.0 / Camera->RollingShutterRate;
    double T = 1.0 / Fs;
    double NFFT = 1024;
    double gain = 5;

    std::vector<double> EstimatedFrequencies;


    for (unsigned i = 0; i < Contours->size(); ++i) {
      int Left = MAX(0, (int)((*Centers)[i].x - (*Radiuses)[i]));
      int Top = MAX(0, (int)((*Centers)[i].y - (*Radiuses)[i]));
      int Right = MIN((int)(GrayImage.size[1])-1, (int)((*Centers)[i].x + (*Radiuses)[i]));
      int Bottom = MIN((int)(GrayImage.size[0])-1, (int)((*Centers)[i].y + (*Radiuses)[i]));
      cv::rectangle(SliceImage, cv::Rect(Left, Top, Right - Left, Bottom - Top), cv::Scalar(255, 255, 255), 5);
      cv::Mat SubImage=GrayImage(cv::Rect(Left, Top, Right - Left, Bottom - Top)).clone();
      //vlc::Tools::ShowImage("SubImage"+std::to_string(i), SubImage);
      cv::Mat ReduceImage;
      cv::reduce(SubImage, ReduceImage, 0, CV_REDUCE_SUM,CV_32FC1);
      double ReduceImageMean = cv::mean(ReduceImage).val[0];
      ReduceImage.convertTo(ReduceImage, CV_32FC1,1,-ReduceImageMean);
      vlc::Tools::Plot("SubImage"+std::to_string(i), ReduceImage,cv::Scalar(0,0,255));
      cv::Mat FFTImage;
      cv::dft(ReduceImage, FFTImage);
      vlc::Tools::Plot("SubImage"+std::to_string(i), FFTImage,cv::Scalar(0,255,0));
      cv::Point MaxLoc;
      cv::minMaxLoc(FFTImage, NULL, NULL, NULL, &MaxLoc);
      double Frequency = (Fs / 2.0) * (1.0*MaxLoc.x / FFTImage.size[1]);
      cv::putText(SliceImage, std::to_string(Frequency), (*Centers)[i], CV_FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255));
      vlc::Tools::PrintMessage("SliceImage", std::to_string(Frequency));
      //vlc::Tools::ShowImage("ReduceImage"+std::to_string(i), ReduceImage);
    }

    vlc::Tools::ShowImage("SliceImage", SliceImage);

  }

  Processor::Processor()
  {
  }


  Processor::~Processor()
  {
  }
}
