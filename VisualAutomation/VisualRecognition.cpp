#include <thread>
#include <process.h>
#include <afxwin.h>         // MFC core and standard components
#include "stdafx.h"
#include "VisualRecognition.h"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


using namespace cv;
using namespace std;

VideoCapture cap;
Mat src, src_gray, src_canny;
bool isRender = false;


VisualRecognition::VisualRecognition()
{

}


VisualRecognition::~VisualRecognition()
{
}
void __cdecl VisualRecognition::render(void * parg)
{
	
	while (isRender==true)
	{
		cap >> src;
		if (!src.empty())
		{
			imshow("CONTOUR", src);
		}
		else
		{
			AfxMessageBox(_T("CAMERA NOT CONNECTED!"));
			isRender = false;
			break;
		}
		int key = waitKey(10);
		if (key == 27) break;
	}
	Beep(('A') * 100, 175);
	_endthread();
}
int VisualRecognition::showWnd(CImage *m_img, CStatic s_img)
{
	isRender = true;
	//************** CAMERA
	if (!cap.isOpened())
	{
		cap.open(1);
		cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
		cap.set(CV_CAP_PROP_SHARPNESS, 20);
		cap.set(CV_CAP_PROP_FPS, 4);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
		//cap.open("video.MP4");
	}
	if (!cap.isOpened())
	{	
		AfxMessageBox(_T("CAMERA NOT CONNECTED!"));
		return(0);
	}

	cap >> src;

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

	if (src.empty())
	{
		AfxMessageBox(_T("IMAGE SOURCE IS EMPTY!"));
		return(-2);
	}
	else if (src.channels()>1)
		cvtColor(src, src_gray, CV_BGR2GRAY);
	else src_gray = src;
	/*
	if (m_img)
	{
		m_img->ReleaseDC();
		delete m_img;
		m_img = NULL;
	}

	Size m_sizeShow = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	m_img->Create(m_sizeShow.width, m_sizeShow.height, 24);

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
	StretchDIBits(m_img->GetDC(), 0, 0, m_sizeShow.width, m_sizeShow.height, 0, 0, m_sizeShow.width, m_sizeShow.height, src.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	m_img->BitBlt(::GetDC(s_img.m_hWnd), 0, 0);
	*/
	//************ WINDOW **************
	namedWindow("CONTOUR", CV_WINDOW_AUTOSIZE);
	moveWindow("CONTOUR", 0, 0);

	//render();
	//thread t1(&VisualRecognition::render, this);
	_beginthread(&VisualRecognition::render, 0, NULL);

	return(1);
}

void VisualRecognition::destroyWnd()
{
	isRender = false;
	destroyWindow("CONTOUR");
}
