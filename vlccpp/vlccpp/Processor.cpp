#include "stdafx.h"
#include "Processor.h"

namespace vlc {

  std::vector<vlc::Transmitter>* Processor::OpenFFT(cv::Mat Image, CameraInfo* Camera) {
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
    //vlc::Tools::ShowImage("RotateImage", RotateImage);
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

    std::vector<vlc::Transmitter>* Lights = new std::vector<vlc::Transmitter>();


    for (unsigned i = 0; i < Contours->size(); ++i) {
      //for (unsigned i = 0; i < 1; ++i) {
      int Left = MAX(0, (int)((*Centers)[i].x - (*Radiuses)[i]));
      int Top = MAX(0, (int)((*Centers)[i].y - (*Radiuses)[i]));
      int Right = MIN((int)(GrayImage.size[1]) - 1, (int)((*Centers)[i].x + (*Radiuses)[i]));
      int Bottom = MIN((int)(GrayImage.size[0]) - 1, (int)((*Centers)[i].y + (*Radiuses)[i]));
      cv::rectangle(SliceImage, cv::Rect(Left, Top, Right - Left, Bottom - Top), cv::Scalar(255, 255, 255), 5);
      cv::Mat SubImage = RotateImage(cv::Rect(Left, Top, Right - Left, Bottom - Top)).clone();
      cv::Mat SubMask = ThresholdImage(cv::Rect(Left, Top, Right - Left, Bottom - Top)).clone();
      cv::Mat SubMaskedImage;
      SubImage.copyTo(SubMaskedImage, SubMask);
      vlc::Tools::ShowImage("SubImage" + std::to_string(i), SubMaskedImage);
      cv::Mat BGRVector;
      if (true) {
        cv::Mat BGR[3];
        cv::Mat BGRThresholdImage[3];
        cv::Mat BGRVectorSum[3];
        cv::Mat BGRVectorNum;
        cv::Mat BGRVectorAvg[3];
        cv::Mat BGRVectorThreshold[3];
        cv::Mat BGRVectorDenoise[3];
        cv::split(SubMaskedImage, BGR);
        cv::reduce(SubMask, BGRVectorNum, 0, CV_REDUCE_SUM, CV_32FC1);
        for (int ch = 0;ch < 3;++ch) {
          cv::adaptiveThreshold(BGR[ch], BGRThresholdImage[ch], 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 51, -30);
          cv::reduce(BGRThresholdImage[ch], BGRVectorSum[ch], 0, CV_REDUCE_SUM, CV_32FC1);
          cv::Mat VectorAvg = BGRVectorSum[ch] / BGRVectorNum;
          VectorAvg.convertTo(BGRVectorAvg[ch], CV_8U);
          cv::threshold(BGRVectorAvg[ch], BGRVectorThreshold[ch], 32, 255, cv::THRESH_BINARY);
          cv::erode(BGRVectorThreshold[ch], BGRVectorDenoise[ch], cv::Mat());
          cv::dilate(BGRVectorDenoise[ch], BGRVectorDenoise[ch], cv::Mat());
          cv::dilate(BGRVectorDenoise[ch], BGRVectorDenoise[ch], cv::Mat());
          cv::erode(BGRVectorDenoise[ch], BGRVectorDenoise[ch], cv::Mat());
        }
        cv::merge(std::vector<cv::Mat>({ BGRVectorDenoise[0],BGRVectorDenoise[1],BGRVectorDenoise[2] }), BGRVector);

        //vlc::Tools::ShowImage("RImage" + std::to_string(i), BGR[2]);
        //vlc::Tools::ShowImage("RThresholdImage" + std::to_string(i), BGRThresholdImage[2]);
        //vlc::Tools::ShowImage("GImage" + std::to_string(i), BGR[1]);
        //vlc::Tools::ShowImage("GThresholdImage" + std::to_string(i), BGRThresholdImage[1]);
        //vlc::Tools::ShowImage("BImage" + std::to_string(i), BGR[0]);
        //vlc::Tools::ShowImage("BThresholdImage" + std::to_string(i), BGRThresholdImage[0]);

      }

      if (false) {
        cv::Mat HlsImage;
        cv::cvtColor(SubMaskedImage, HlsImage, CV_BGR2HLS);
        cv::Mat HLS[3];
        cv::split(HlsImage, HLS);
        cv::Mat H = HLS[0];
        cv::Mat L = HLS[1];
        cv::Mat S = HLS[2];
        cv::Mat LHigh;
        cv::Mat LLow;
        cv::Mat HRed;
        cv::Mat HBlue;
        cv::Mat HGreen;
        vlc::Tools::ShowImage("HImage" + std::to_string(i), H);
        vlc::Tools::ShowImage("LImage" + std::to_string(i), L);
        vlc::Tools::ShowImage("SImage" + std::to_string(i), S);
        cv::adaptiveThreshold(L, LHigh, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 21, -64);
        //cv::inRange(L, cv::Scalar(255 * 0.6), cv::Scalar(255 * 1), LHigh);
        cv::adaptiveThreshold(L, LLow, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 21, 64);
        //cv::inRange(L, cv::Scalar(255 * 0.15), cv::Scalar(255 * 1), LLow);
        vlc::Tools::ShowImage("LHighImage" + std::to_string(i), LHigh);
        vlc::Tools::ShowImage("LLowImage" + std::to_string(i), LLow);

        cv::Mat HVectorSum, HVectorNum;
        cv::Mat HVecImage;
        cv::reduce(H, HVectorSum, 0, CV_REDUCE_SUM, CV_32FC1);
        cv::reduce(SubMask, HVectorNum, 0, CV_REDUCE_SUM, CV_32FC1);
        cv::Mat HVectorAvg = HVectorSum / 256 / HVectorNum;
        printf("cols:%d,rows:%d\n", HVectorSum.cols, HVectorSum.rows);
        printf("Sum:%f,Num:%f,Avg:%f\n", HVectorSum.at<float>(230), HVectorNum.at<float>(230), HVectorAvg.at<float>(230));
        //cv::repeat(HVectorAvg, 50, 1, HVecImage);
        vlc::Tools::ShowImage("HVectorAvg" + std::to_string(i), HVectorAvg);
        cv::Mat HVectorMost;
        vlc::Tools::ReduceRowByMost(H, HVectorMost, SubMask);
        vlc::Tools::ShowImage("HVectorMost" + std::to_string(i), HVectorMost);

        HInRange(H, 0, 45, HRed);
        //cv::bitwise_or(HRed, LHigh,HRed);
        //cv::bitwise_and(HRed, LLow,HRed);
        vlc::Tools::ShowImage("HRedImage" + std::to_string(i), HRed);
        cv::Mat HRed2;
        HInRange(HVectorMost, 0, 45, HRed2);
        vlc::Tools::ShowImage("HRed2Image" + std::to_string(i), HRed2);

      }
      cv::Mat BGRVectorImage;
      BGRVectorImage.create(4, SubImage.cols, CV_8UC3);
      cv::Mat EmptyChannel;
      EmptyChannel.create(1, SubImage.cols, CV_8UC1);
      EmptyChannel.setTo(cv::Scalar(0));
      cv::Mat BGRVectorRow;
      cv::Mat BGRVectors[3];
      cv::split(BGRVector, BGRVectors);
      cv::merge(std::vector<cv::Mat>({ EmptyChannel,EmptyChannel, BGRVectors[2] }), BGRVectorRow);
      BGRVectorRow.copyTo(BGRVectorImage(cv::Rect(0, 0, BGRVectorRow.cols, BGRVectorRow.rows)));
      cv::merge(std::vector<cv::Mat>({ EmptyChannel, BGRVectors[1],EmptyChannel }), BGRVectorRow);
      BGRVectorRow.copyTo(BGRVectorImage(cv::Rect(0, 1, BGRVectorRow.cols, BGRVectorRow.rows)));
      cv::merge(std::vector<cv::Mat>({ BGRVectors[0],EmptyChannel,EmptyChannel }), BGRVectorRow);
      BGRVectorRow.copyTo(BGRVectorImage(cv::Rect(0, 2, BGRVectorRow.cols, BGRVectorRow.rows)));
      BGRVector.copyTo(BGRVectorImage(cv::Rect(0, 3, BGRVectorRow.cols, BGRVectorRow.rows)));
      vlc::Tools::ShowImage("BGRVectorImage" + std::to_string(i), BGRVectorImage);
      int RedRise = 0, RedFall = 0;
      uchar RedPrevVal = 0, RedNowVal = 0;
      std::string BlueData = "", GreenData = "";
      for (int k = 0;k < SubImage.cols;++k) {
        RedNowVal = BGRVectors[2].data[k];
        if (RedPrevVal == 0 && RedNowVal > 0) {
          RedRise = k;
        }
        else if (RedPrevVal > 0 && RedNowVal == 0) {
          RedFall = k;
          cv::Scalar NowVal = cv::mean(BGRVector(cv::Rect(RedRise, 0, RedFall - RedRise, 1)));
          GreenData += (NowVal[1] > 128 ? '1' : '0');
          BlueData += (NowVal[0] > 128 ? '1' : '0');
        }
        RedPrevVal = RedNowVal;
      }
      vlc::Tools::PrintMessage("BGRVectorImage" + std::to_string(i), BlueData + "[Blue]");
      vlc::Tools::PrintMessage("BGRVectorImage" + std::to_string(i), GreenData + "[Green]");

      int start = GreenData.find("11010000");
      if (start == GreenData.npos) {
        continue;
      }
      unsigned int ID = Tools::ConvertFromBinToDec(BlueData.substr(start, 8));

      vlc::Transmitter Light;
      Light.PositionInImage = (*Centers)[i];
      Light.ID = ID;
      Lights->push_back(Light);
      cv::putText(SliceImage, std::to_string(ID), (*Centers)[i], CV_FONT_HERSHEY_SIMPLEX, 20, cv::Scalar(255, 255, 255),20);
      vlc::Tools::PrintMessage("SliceImage", std::to_string(ID));
    }
    vlc::Tools::ShowImage("SliceImage", SliceImage);
    return Lights;
  }
  void Processor::HInRange(cv::InputArray src, double HCenter, double HError, cv::OutputArray dst) {
    double HLow = HCenter - HError;
    double HHigh = HCenter + HError;
    double HMAX = 180;
    double HCeilLow = 0;
    double HCeilHigh = 0;
    bool FlagCeil = false;
    double HFloorLow = 0;
    double HFloorHigh = 0;
    bool FlagFloor = false;
    if (HLow < 0) {
      HCeilLow = HLow + HMAX;
      HCeilHigh = HMAX;
      HLow = 0;
      FlagCeil = true;
    }
    if (HHigh > HMAX) {
      HFloorHigh = HHigh - HMAX;
      HFloorLow = 0;
      HHigh = HMAX;
      FlagFloor = true;
    }
    cv::inRange(src, cv::Scalar(HLow), cv::Scalar(HHigh), dst);
    if (FlagCeil) {
      cv::Mat tmpMat;
      cv::inRange(src, cv::Scalar(HCeilLow), cv::Scalar(HCeilHigh), tmpMat);
      cv::bitwise_or(dst, tmpMat, dst);
    }
    if (FlagFloor) {
      cv::Mat tmpMat;
      cv::inRange(src, cv::Scalar(HFloorLow), cv::Scalar(HFloorHigh), tmpMat);
      cv::bitwise_or(dst, tmpMat, dst);
    }
  }

  void Processor::PairLights(std::vector<vlc::Transmitter>* Lights, RoomInfo * Room)
  {
    char cstr[MAX_PATH];
    unsigned n = Lights->size();
    vlc::Transmitter* LightsArray = new vlc::Transmitter[n];
    for (unsigned i = 0; i < n; ++i)
      LightsArray[i] = (*Lights)[i];
    vlc::Transmitter tmpLight;
    for (unsigned i = 0; i < n; ++i) {
      for (unsigned j = i + 1; j < n; ++j) {
        if (LightsArray[i].Frequency > LightsArray[j].Frequency) {
          tmpLight = LightsArray[i];
          LightsArray[i] = LightsArray[j];
          LightsArray[j] = tmpLight;
        }
      }
    }
    n = MIN(n, Room->Transmitters.size());
    Lights->clear();
    for (unsigned i = 0; i < n; i++) {
      double Frequencies[] = { 2000,2500,3000,3500,4000 };
      LightsArray[i].PositionInRoom = (Room->Transmitters[Frequencies[i]]);
      Lights->push_back(LightsArray[i]);
      sprintf(cstr, "%0.1lf (%0.1lf,%0.1lf) -> (%0.1lf,%0.1lf,%0.1lf)", LightsArray[i].Frequency, LightsArray[i].PositionInImage.x, LightsArray[i].PositionInImage.y, LightsArray[i].PositionInRoom.x, LightsArray[i].PositionInRoom.y, LightsArray[i].PositionInRoom.z);
      vlc::Tools::PrintMessage("Pair", cstr);
    }
  }

  void Processor::AOA(std::vector<vlc::Transmitter>* Lights, CameraInfo * Camera)
  {


  }

  Processor::Processor()
  {
  }


  Processor::~Processor()
  {
  }
}
