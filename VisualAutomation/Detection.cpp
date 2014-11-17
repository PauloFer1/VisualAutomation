#include "stdafx.h"
#include "Detection.h"
#include <thread>
#include <process.h>
#include <afxwin.h>         // MFC core and standard components

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Camera.h"

using namespace cv;
using namespace std;


Detection::Detection()
{
	hasCam = false;
	isRender = false;
	flag = 0;
	//  setBlur = 0;
	//  setThreshold = 0;
	blurValue = 0;
	thresholdValue = 0;
	imgWidth = 0;
	imgHeight = 0;
}


Detection::~Detection()
{
	instanceFlag = false;
}

bool Detection::instanceFlag = false;
Detection* Detection::single = NULL;

Detection* Detection::getInstance()
{
	if (!instanceFlag)
	{
		single = new Detection();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}
void __cdecl Detection::render(void * parg)
{

	while (Detection::getInstance()->isRender == true)
	{
		Camera::getInstance()->getCapture() >> Detection::getInstance()->src;
		if (!Detection::getInstance()->src.empty())
		{
			if (Detection::getInstance()->m_img)
			{
				Detection::getInstance()->m_img.ReleaseDC();
				Detection::getInstance()->deleteImg();
			}

			Mat tmp, tmp2;
			flip(Detection::getInstance()->src, tmp, 0);
			resize(tmp, tmp2, Size(640,420), 0, 0, INTER_AREA);
			//Size m_sizeShow = Size(Detection::getInstance()->src.size().width, Detection::getInstance()->src.size().height);
			Size m_sizeShow = Size(640  , 420);
			Detection::getInstance()->m_img.Create(m_sizeShow.width, m_sizeShow.height, 24);

			BITMAPINFO bitInfo;
			bitInfo.bmiHeader.biBitCount = 24;
			bitInfo.bmiHeader.biWidth = m_sizeShow.width;
			bitInfo.bmiHeader.biHeight = m_sizeShow.height;
			bitInfo.bmiHeader.biPlanes = 1;
			bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitInfo.bmiHeader.biCompression = BI_RGB;
			bitInfo.bmiHeader.biClrImportant =
				bitInfo.bmiHeader.biClrUsed =
				bitInfo.bmiHeader.biSizeImage =
				bitInfo.bmiHeader.biXPelsPerMeter =
				bitInfo.bmiHeader.biYPelsPerMeter = 0;
			StretchDIBits(Detection::getInstance()->m_img.GetDC(), 0, 0, m_sizeShow.width, m_sizeShow.height, 0, 0, m_sizeShow.width, m_sizeShow.height, tmp2.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
			Detection::getInstance()->m_img.BitBlt(::GetDC(Detection::getInstance()->imgWnd->m_hWnd), 0, 0);
			//imshow("CONTOUR", Detection::getInstance()->src);
			
			Detection::getInstance()->renderCanny(Detection::getInstance()->processImage());

			tmp.release();
			tmp2.release();
			Detection::getInstance()->src.release();
		}
		else
		{
			AfxMessageBox(_T("Detection::render::CAMERA NOT CONNECTED!"));
			Detection::getInstance()->isRender = false;
			break;
		}
	//	int key = waitKey(10);
	//	if (key == 27) break;
		Sleep(5);

	}
	Beep(('A') * 100, 175);
	_endthread();
	Detection::getInstance()->isRender = false;
}
int Detection::showWnd()
{
	if (!Camera::getInstance()->getCapture().isOpened())
	{
		AfxMessageBox(_T("Detection::showWnd::CAMERA NOT CONNECTED!"));
		return(-2);
	}
	isRender = true;

	Sleep(50);
	Camera::getInstance()->getCapture() >> src;
	if (src.empty())
	{
		AfxMessageBox(_T("Detection::showWnd::IMAGE SOURCE IS EMPTY!"));
	//	return(-2);
	}
	else if (src.channels()>1)
		cvtColor(src, src_gray, CV_BGR2GRAY);
	else src_gray = src;

	imgWidth = src.size().width;
	imgHeight = src.size().height;
	
	//************ WINDOW **************
	//namedWindow("CONTOUR", CV_WINDOW_AUTOSIZE);
	//moveWindow("CONTOUR", 0, 0);
	namedWindow("1", CV_WINDOW_AUTOSIZE);

	//render();
	//thread t1(&VisualRecognition::render, this);
	_beginthread(&Detection::render, 0, NULL);

	return(1);
}

void Detection::destroyWnd()
{
	isRender = false;
//	destroyWindow("CONTOUR");
}
void Detection::method()
{
	AfxMessageBox(L"Hello!");
}


int Detection::closeCam()
{
	cap.release();
	return 1;
}


int Detection::deleteImg()
{
	m_img.Destroy();
	return 0;
}


int Detection::setThreshold(int value)
{
	thresholdValue = value;
	return 0;
}


int Detection::setBlur(int value)
{
	blurValue = value;
	return 0;
}


Mat Detection::processImage()
{
	Mat canny_output, gray_clone, blured, lap, src_gray, clone;

	if (src.empty())
		return(canny_output);
	else if (src.channels()>1)
		cvtColor(src, src_gray, CV_BGR2GRAY);
	else src_gray = src;

	if (blurValue != 0)
		blur(src_gray, blured, Size(blurValue, blurValue));
	else
		blured = src_gray;

	//blured.convertTo(lap, -1, alpha, beta);

	gray_clone = blured.clone();

	Canny(blured, canny_output, thresholdValue, thresholdValue * 2, 3);

	//»»»»»»»» CENTER CROSS
	clone = src.clone();
	Point c = Point(imgWidth / 2, imgHeight / 2);
	Point m1 = Point(imgWidth / 2 - 20, imgHeight / 2);
	Point m2 = Point(imgWidth / 2 + 20, imgHeight / 2);
	Point m3 = Point(imgWidth / 2, imgHeight / 2 - 20);
	Point m4 = Point(imgWidth / 2, imgHeight / 2 + 20);
	line(clone, m1, m2, Scalar(0, 255, 255), 1, CV_AA);
	line(clone, m3, m4, Scalar(0, 255, 255), 1, CV_AA);

	int rowM, rowL, rowR, rowB, colL, colR, col2L, col2R = 0;
	const int INC_HEIGHT = 60;

	//return canny_output;
	gray_clone.release();
	lap.release();
	//src_gray.release();
	clone.release();

	
	imshow("1", blured);

	return(src_gray);
}


int Detection::renderCanny(Mat canny)
{
	if (Detection::getInstance()->m_canny)
	{
		Detection::getInstance()->m_canny.ReleaseDC();
		Detection::getInstance()->m_canny.Destroy();;
	}

	Mat tmp, tmp2;
	flip(canny, tmp, 0);
	Size m_sizeShow = Size(640 / 4, 420 / 4);
	//Size m_sizeShow = Size(320, 210);
	resize(tmp, tmp2, m_sizeShow, 0, 0);
	Detection::getInstance()->m_canny.Create(m_sizeShow.width, m_sizeShow.height, 24);

	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 24;
	bitInfo.bmiHeader.biWidth = m_sizeShow.width;
	bitInfo.bmiHeader.biHeight = m_sizeShow.height;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant =
		bitInfo.bmiHeader.biClrUsed =
		bitInfo.bmiHeader.biSizeImage =
		bitInfo.bmiHeader.biXPelsPerMeter =
		bitInfo.bmiHeader.biYPelsPerMeter = 0;
	StretchDIBits(Detection::getInstance()->m_canny.GetDC(), 0, 0, m_sizeShow.width, m_sizeShow.height, 0, 0, m_sizeShow.width, m_sizeShow.height, tmp2.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	Detection::getInstance()->m_canny.BitBlt(::GetDC(Detection::getInstance()->cannyWnd->m_hWnd), 0, 0);

	tmp.release();
	tmp2.release();

	return 0;
}
