#include "stdafx.h"
using namespace std;

#define NOT_VALID_IMAGE 1

int main(int argc, char** argv)
{
  char cstr[MAX_PATH];

  GetCurrentDirectoryA(MAX_PATH, cstr);
  string CurrentDirectory = cstr;
  string ImageFilename = CurrentDirectory + "\\..\\..\\images\\20150604\\4.jpg";
  cv::Mat image;
  image = cv::imread(ImageFilename, cv::IMREAD_COLOR); // Read the file

  if (!image.data) // Check for invalid input
  {
    printf(ImageFilename.c_str());
    return NOT_VALID_IMAGE;
  }

  vlc::CameraInfo* Lumia1020 = new vlc::CameraInfo();
  Lumia1020->Focal = 5620;
  Lumia1020->RollingShutterRate = 1 / 47.54e3;

  vlc::CameraInfo* Camera = Lumia1020;

  vlc::RoomInfo* SJTULab = new vlc::RoomInfo();
  SJTULab->Transmitters[1] = cv::Point3d(-0.5, 0, 0);
  SJTULab->Transmitters[2] = cv::Point3d(0, 0.5, 0);
  SJTULab->Transmitters[3] = cv::Point3d(0, 0, 0);
  SJTULab->Transmitters[4] = cv::Point3d(0.5, 0, 0);
  SJTULab->Transmitters[5] = cv::Point3d(0, -0.5, 0);

  vlc::RoomInfo* Room = SJTULab;

  vector<double> Frequencies;
  for (map<unsigned, cv::Point3d>::const_iterator iter = Room->Transmitters.begin(); iter != Room->Transmitters.end(); ++iter) {
    Frequencies.push_back(iter->first);
  }

  vlc::Tools::ShowImage("Origin", image);
  for (map<unsigned, cv::Point3d>::const_iterator iter = Room->Transmitters.begin(); iter != Room->Transmitters.end(); ++iter) {
    sprintf(cstr, "%d (%0.1lf,%0.1lf,%0.1lf)", iter->first, iter->second.x, iter->second.y, iter->second.z);
    vlc::Tools::PrintMessage("Origin", cstr);
  }
  vlc::Transmitter t;
  std::vector<vlc::Transmitter>* Lights;
  Lights = vlc::Processor::OpenFFT(image, Camera);
  vlc::Processor::PairLights(Lights, Room);


  cv::waitKey(0); // Wait for a keystroke in the window
  return 0;
}
