#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Constants.h"

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
	bool hasCam;
	CStatic *imgWnd;
	CImage m_img;
	int blurValue;
	int thresholdValue;
	CImage m_canny;
	CStatic* cannyWnd;
	Scalar filterColor;
	int thresholdProcValue;
	int typeThreshold;
	int thresholdFunction;
	bool useThreshold;
	int calibValue;
	bool hasVision=false;
	bool canRead = true;
public:
	int initCam();
	int closeCam();
	int deleteImg();
	int setThreshold(int value);
	int setBlur(int value);
	int processImage();
	int showCanny();
	int showProcImage();
	int closeCanny();
	int closeProcImage();
	int renderProcImage(Mat img);
	int setFilterColor(int r, int g, int b);
	int applyContrast(int value);
	Mat thresholdImg(int value, Mat src, int type);
	int setThresholdValue(int value);
	int removeNoiseMat(Mat * imgs, Mat dst);
	void checkLines(Mat src, Mat dst, vector<Vec4i> lines);
	int travelPixels(Mat img);
	int pixelsToMm(int px);
	int mmToPixels(int mm);
	int calibrateMM();
	void drawGripper(Mat img, Point2i center, Rect rect, float angle);
	void calculateGripper(int x, int y);
	int detectGap(int it, int* px);
	Point2i getOffset();
	void startVision();
	void enableRead();
	__event void enableOut(int i);
	__event void enableRead(int i);
private:
	int imgWidth;
	int imgHeight;
	int renderCanny(Mat canny);
	bool isCannyWnd;
	bool isProcWnd;
	int detectGap(int* it, int* pos, int* gap);
	int detectGapVer(int it, int pos, int gap);
	HANDLE renderSemaphore;
	vector<Point2i> sample;
	int sampleSize = 6;
};

