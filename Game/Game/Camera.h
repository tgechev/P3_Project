#ifndef CAMERA_H
#define CAMERA_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<mutex>

class Camera {
private:
	cv::VideoCapture camStream;
	std::mutex myMutex;
public:
	Camera();
	cv::Mat getCameraFrame();
	cv::VideoCapture getStream();
	void closeStream();
};
#endif
