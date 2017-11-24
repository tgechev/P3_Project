#include "Camera.h"

cv::Mat Camera::getCameraFrame() {
	cv::Mat camFrame, videoFrame;

	myMutex.lock();
	camStream.read(camFrame);
	videoFrame = camFrame.clone();
	myMutex.unlock();

	return videoFrame;
}
Camera::Camera() {
	camStream.open(1);
	if (!camStream.isOpened()) {
		camStream.open(0);
	}
}
cv::VideoCapture Camera::getStream() {
	return camStream;
}
void Camera::closeStream() {
	camStream.release();
}