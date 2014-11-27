#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
using namespace std;

class Camera
{
private:
	static bool instanceFlag;
	static Camera *single;
public:
	Camera();
	virtual ~Camera();
	static Camera* getInstance();
	VideoCapture getCapture();
private:
	VideoCapture cap;
public:
	int initCamera();
	int closeCamera();
	int getCameraList();
	bool hasCamera;
	int setBrightness(int value);
	int setExposure(int value);
	int setConstrast(int value);
	int setZoom(int value);
};

