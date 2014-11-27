#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	hasCamera = false;
}


Camera::~Camera()
{
	hasCamera = false;
	instanceFlag = false;
}

bool Camera::instanceFlag = false;
Camera* Camera::single = NULL;

Camera* Camera::getInstance()
{
	if (!instanceFlag)
	{
		single = new Camera();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}

VideoCapture Camera::getCapture()
{
	return cap;
}


int Camera::initCamera()
{
	if (!cap.isOpened())
	{
		bool cam;
		cap.open(0);
		cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
		cap.set(CV_CAP_PROP_SHARPNESS, 20);
		cap.set(CV_CAP_PROP_FPS, 4);
		cam = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1920);
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
		//cap.open("video.MP4");
	}
	if (!cap.isOpened())
	{
		AfxMessageBox(_T("CAMERA NOT CONNECTED!"));
		return(-1);
	}
	Sleep(5);
	hasCamera = true;
	return 1;
}


int Camera::closeCamera()
{
	cap.release();
	hasCamera = false;
	return 1;
}


int Camera::getCameraList()
{
	
	return 0;
}


int Camera::setBrightness(int value)
{
	cap.set(CV_CAP_PROP_BRIGHTNESS, value);
	return 1;
}


int Camera::setExposure(int value)
{
	cap.set(CV_CAP_PROP_EXPOSURE, value);
	return 1;
}
int Camera::setConstrast(int value)
{
	cap.set(CV_CAP_PROP_CONTRAST, value);
	return(1);
}


int Camera::setZoom(int value)
{
	cap.set(CV_CAP_PROP_ZOOM, value);
	return(1);
}
