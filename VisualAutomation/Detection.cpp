#include "stdafx.h"
#include "Detection.h"
#include <thread>
#include <process.h>
#include <afxwin.h> // MFC core and standard components
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\photo\photo.hpp"
#include "Camera.h"

using namespace cv;

using namespace std;
Detection::Detection()
{
	hasCam = false;
	isRender = false;
	flag = 0;
	// setBlur = 0;
	// setThreshold = 0;
	blurValue = 0;
	thresholdValue = 0;
	imgWidth = 0;
	imgHeight = 0;
	filterColor = Scalar(0, 0, 0);
	thresholdProcValue = 0;
	typeThreshold = ADAPTIVE_THRESH_GAUSSIAN_C;
	thresholdFunction = 0;
	useThreshold = false;
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
	Mat ids = Mat(Camera::getInstance()->getCameraHeight(), Camera::getInstance()->getCameraWidth(), CV_8UC1, (UCHAR*)(Camera::getInstance()->getFrame()));
	cvtColor(ids, ids, CV_GRAY2RGB);
	Detection::getInstance()->src = ids;

	if (!Detection::getInstance()->src.empty())
	{
		if (Detection::getInstance()->m_img)
		{
			Detection::getInstance()->m_img.ReleaseDC();
			Detection::getInstance()->deleteImg();
		}
		Detection::getInstance()->processImage();
		Mat tmp, tmp2;
		flip(Detection::getInstance()->src, tmp, 0);
		//flip(srcTmp, tmp, 0);
		resize(tmp, tmp2, Size(ids.size().width/2,ids.size().height/2), 0, 0, INTER_AREA);
		//Size m_sizeShow = Size(Detection::getInstance()->src.size().width, Detection::getInstance()->src.size().height);
		Size m_sizeShow = Size(ids.size().width / 2, ids.size().height / 2);
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
		//Detection::getInstance()->processImage();
		tmp.release();
		tmp2.release();
		ids.release();
		Detection::getInstance()->src.release();
	}
	else
	{
		AfxMessageBox(_T("Detection::render::CAMERA NOT CONNECTED!"));
		Detection::getInstance()->isRender = false;
		break;
	}
	// int key = waitKey(10);
	// if (key == 27) break;
	Sleep(100);
	
	Detection::getInstance()->processImage();
	}
	Beep(('A') * 100, 175);
	_endthread();
	Detection::getInstance()->isRender = false;
}
int Detection::showWnd()
{
	if (!Camera::getInstance()->isOpen())
	{
		AfxMessageBox(_T("Detection::showWnd::CAMERA NOT CONNECTED!"));
		return(-2);
	}
	isRender = true;
	Sleep(50);

	src = Mat(Camera::getInstance()->getCameraHeight(), Camera::getInstance()->getCameraWidth(), CV_8UC1, (UCHAR*)(Camera::getInstance()->getFrame()));
	if (src.empty())
	{
		AfxMessageBox(_T("Detection::showWnd::IMAGE SOURCE IS EMPTY!"));
		return(-2);
	}
	else if (src.channels()>1)
		cvtColor(src, src_gray, CV_BGR2GRAY);
	else src_gray = src;
	
	imgWidth = src.size().width;
	imgHeight = src.size().height;
	//************ WINDOW **************
	// namedWindow("DEBUG", CV_WINDOW_AUTOSIZE);
	//namedWindow("CONTOUR", CV_WINDOW_AUTOSIZE);
	//moveWindow("CONTOUR", 0, 0);
	//render();
	//thread t1(&VisualRecognition::render, this);
	_beginthread(&Detection::render, 0, NULL);
	return(1);
}
void Detection::destroyWnd()
{
isRender = false;
// destroyWindow("CONTOUR");
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
int Detection::processImage()
{
	Mat canny_output, blured, lap, src_gray, clone, redOnly;
	if (src.empty())
		return(-2);
	else if (src.channels()>1)
		cvtColor(~src, src_gray, CV_BGR2GRAY);
	else src_gray = src;
	if (blurValue != 0)
		blur(src_gray, blured, Size(blurValue, blurValue));
	else
		blured = src_gray;
	if (thresholdProcValue != 0)
		blured = thresholdImg(thresholdProcValue, blured, 0);

	//removeNoiseMat(sample, blured);
	//blured.convertTo(lap, -1, alpha, beta);
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
	//**RED
	//inRange(src, filterColor, filterColor, redOnly);
	
	if (isCannyWnd)
		renderCanny(canny_output);
	if (isProcWnd)
	{
		renderProcImage(blured);
	}
	//vector<Vec4i> lines;
	//checkLines(blured, src, lines);
	lap.release();
	src_gray.release();
	clone.release();
	blured.release();
	canny_output.release();
	redOnly.release();
	
	return(1);
}
int Detection::renderCanny(Mat canny)
{
	HWND hwnd = (HWND)cvGetWindowHandle("CANNY IMAGE");
	if (!IsWindowVisible(hwnd)) {
		closeCanny();
		return(0);
	}
	imshow("CANNY IMAGE", canny);
	return 1;
}
int Detection::renderProcImage(Mat img)
{
	HWND hwnd = (HWND)cvGetWindowHandle("PROCESSED IMAGE");
	if (!IsWindowVisible(hwnd)) {
		closeProcImage();
		return(0);
	}
	imshow("PROCESSED IMAGE", img);
	return 1;
}
int Detection::showCanny()
{
	namedWindow("CANNY IMAGE", CV_WINDOW_AUTOSIZE);
	moveWindow("CANNY IMAGE", 0, 0);
	isCannyWnd = true;
	return 1;
}
int Detection::showProcImage()
{
	namedWindow("PROCESSED IMAGE", CV_WINDOW_AUTOSIZE);
	moveWindow("PROCESSED IMAGE", 0, 0);
	isProcWnd = true;
	return 0;
}
int Detection::closeCanny()
{
	destroyWindow("CANNY IMAGE");
	isCannyWnd = false;
	return 0;
}
int Detection::closeProcImage()
{
	destroyWindow("PROCESSED IMAGE");
	isProcWnd = false;
	return 0;
}
int Detection::setFilterColor(int r, int g, int b)
{
	filterColor = Scalar(b, g, r);
	return 1;
}
int Detection::applyContrast(int value)
{
	return 0;
}
Mat Detection::thresholdImg(int value, Mat src, int type)
{
	Mat tmp;
	if (useThreshold)
	{
	if (thresholdFunction == 0)
	adaptiveThreshold(src, tmp, value, typeThreshold, CV_THRESH_BINARY_INV, 7, 3);
	else
	threshold(src, tmp, value, 255, THRESH_BINARY_INV);
	int morph_size = 1;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), Point(morph_size, morph_size));
	morphologyEx(tmp, tmp, MORPH_CLOSE, element);
	//morphologyEx(tmp, tmp, MORPH_OPEN, element);
	return tmp;
}
else
	return(src);
}
int Detection::setThresholdValue(int value)
{
	thresholdProcValue = value;
	return 0;
}
int Detection::removeNoiseMat(Mat * imgs, Mat dst)
{
	Mat tmp[5] = { imgs[0], imgs[1], imgs[2], imgs[3], imgs[4] };
	//fastNlMeansDenoisingMulti(tmp[0], dst, 3, 3, 7, 21);
	return 1;
}
void Detection::checkLines(Mat src, Mat dst, vector<Vec4i> lines)
{
	HoughLinesP(src, lines, 1, CV_PI / 180, 50, 100, 4);
	Vec4i upper = Vec4i(src.size().height, 0, 0, 0);
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		Point c = Point(400, 400);
		Point media = Point((pt2.x + pt1.x) / 2, (pt2.y + pt1.y) / 2);
		float d = sqrt((media.x - c.x)*(media.x - c.x) + (media.y - c.y)*(media.y - c.y));
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		//if ((theta >-1 && theta <1))// || (theta>1.54 && theta<1.6))
		Vec4i l = lines[i];
		//Vertical
		if (abs(l[0] - l[2]) < 5 && l[0]<src.size().width/2 + 20 && l[0]>src.size().width/2 - 20)
		{
			line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
		}
		//Horizontal
		if (abs(l[1] - l[3]) < 8)
		{
			line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
			if (l[0] < src.size().height/4 || l[0] > (3/4)*src.size().height )
			{
				if (l[0] < upper[0])
				upper = l;
			}
		}
	}
	if (upper[1] != 0)
	{
		Mat roi;
		Rect crop = Rect(0, (upper[0]), dst.size().width, 30);
		src(crop).copyTo(roi);
		// imshow("DEBUG", roi);
		roi.release();
	}
}