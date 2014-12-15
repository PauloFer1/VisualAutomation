#pragma once
#include "uEye.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "version.h"
#include "Constants.h"

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
	// uEye varibles
	HIDS	m_hCam;			// handle to camera
	HWND	m_hWndDisplay;	// handle to diplay window
	INT		m_Ret;			// return value of uEye SDK functions
	INT		m_nColorMode;	// Y8/RGB16/RGB24/REG32
	INT		m_nBitsPerPixel;// number of bits needed store one pixel
	INT		m_nSizeX;		// width of video 
	INT		m_nSizeY;		// height of video
	INT		m_lMemoryId;	// grabber memory - buffer ID
	char*	m_pcImageMemory;// grabber memory - pointer to buffer
	INT     m_nRenderMode;  // render  mode
	SENSORINFO m_sInfo;	    // sensor information struct
	char*						m_pbmpInfo;
	double		m_MaxExposure;
	double		m_Exposure;
	double		m_ExposureInc;
	double		m_MinExposure;

	INT InitCameraIDS(HIDS *hCam, HWND hWnd);
	bool OpenCamera();
	void GetMaxImageSize(INT *pnSizeX, INT *pnSizeY);
public:
	int initCamera();
	int closeCamera();
	int getCameraList();
	bool hasCamera;
	int setBrightness(int value);
	int setExposure(int value);
	int setConstrast(int value);
	int setZoom(int value);
	void captureImg();
	HWND getWndDisplay();
	void setWndDisplay(CStatic* wndD);
	char* getFrame();
	int getCameraWidth();
	int getCameraHeight();
	bool isOpen();
};

