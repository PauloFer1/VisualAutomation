#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
using namespace std;

class Detection
{
private:
	static bool instanceFlag;
	static Detection *single;
public:
	static Detection* getInstance();
	void method();
	Detection();
	virtual ~Detection();
	int showWnd();
	void destroyWnd();
	static void __cdecl render(void * parg);
	int flag;
public:
	bool isRender;
	VideoCapture cap;
	Mat src;
	Mat src_gray;
	Mat src_canny;
	int initCam();
	bool hasCam;
	int closeCam();
	CStatic *imgWnd;
	CImage m_img;
	int deleteImg();
//	int setBlur;
//	int setThreshold;
	int setThreshold(int value);
	int setBlur(int value);
	int blurValue;
	int thresholdValue;
	int processImage();
	CImage m_canny;
private:
	int imgWidth;
	int imgHeight;
	int renderCanny(Mat canny);
public:
	CStatic* cannyWnd;
	int showCanny();
	int showProcImage();
private:
	bool isCannyWnd;
	bool isProcWnd;
public:
	int closeCanny();
	int closeProcImage();
	int renderProcImage(Mat img);
	Scalar filterColor;
	int setFilterColor(int r, int g, int b);
	int applyContrast(int value);
	Mat thresholdImg(int value, Mat src, int type);
	int thresholdProcValue;
	int setThresholdValue(int value);
	int typeThreshold;
	int thresholdFunction;
	int removeNoiseMat(Mat * imgs, Mat dst);
private:
	Mat* sample;
public:
	bool useThreshold;
	void checkLines(Mat src, Mat dst, vector<Vec4i> lines);
};

