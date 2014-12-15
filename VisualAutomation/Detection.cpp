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
			if (Detection::getInstance()->canRead)
				Detection::getInstance()->processImage();
			Mat tmp, tmp2;
			flip(Detection::getInstance()->src, tmp, 0);
			resize(tmp, tmp2, Size(ids.size().width/2,ids.size().height/2), 0, 0, INTER_AREA);
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
		Sleep(2);
	}
	Detection::getInstance()->isRender = false;
	if (Detection::getInstance()->m_img)
	{
		Detection::getInstance()->m_img.ReleaseDC();
		Detection::getInstance()->deleteImg();
	}
	Beep(('A') * 100, 175);
	_endthread();

}
int Detection::showWnd()
{
	if (!Camera::getInstance()->isOpen())
	{
		AfxMessageBox(_T("Detection::showWnd::CAMERA NOT CONNECTED!"));
		return(-2);
	}
	this->calibValue = Constants::getInstance()->getCalibVal();
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

	//blured.convertTo(lap, -1, alpha, beta);
	//Canny(blured, canny_output, thresholdValue, thresholdValue * 2, 3);
	//»»»»»»»» CENTER CROSS
	clone = src.clone();
	Point c = Point(imgWidth / 2, imgHeight / 2);
	Point m1 = Point(imgWidth / 2 - 20, imgHeight / 2);
	Point m2 = Point(imgWidth / 2 + 20, imgHeight / 2);
	Point m3 = Point(imgWidth / 2, imgHeight / 2 - 20);
	Point m4 = Point(imgWidth / 2, imgHeight / 2 + 20);
	line(src, m1, m2, Scalar(0, 255, 255), 1, CV_AA);
	line(src, m3, m4, Scalar(0, 255, 255), 1, CV_AA);
	int rowM, rowL, rowR, rowB, colL, colR, col2L, col2R = 0;
	const int INC_HEIGHT = 60;

	travelPixels(blured);
	
	if (isCannyWnd)
		renderCanny(canny_output);
	if (isProcWnd)
	{
		renderProcImage(blured);
	}
	vector<Vec4i> lines;
	//checkLines(blured, src, lines);
	lap.release();
	src_gray.release();
	clone.release();
	blured.release();
	canny_output.release();
	redOnly.release();
	
	return(1);
}
int Detection::travelPixels(Mat src)
{
	int pxYL=1;
	int pxYR=1;
	int posL = 1;
	int posR = 1;
	int posB = 1;
	int posT = 1;
	//***** FIND HORIZONTAL LINE
	for (int i = (src.rows/2)+20; i > 0; i--)
	{
		if ((int)src.at<uchar>(i, (src.cols/2)+30) > 0)
		{
			Point m1 = Point(imgWidth / 2 - 20, i);
			Point m2 = Point(imgWidth / 2 + 20, i);
			line(this->src, m1, m2, Scalar(0, 0, 255), 1, CV_AA);
			pxYL = i;
			pxYR = i;
			break;
		}
	}
	int lLimit = this->src.cols/2 - this->mmToPixels(Constants::getInstance()->getObjWidth()) / 4;
	int rLimit = this->src.cols/2 + this->mmToPixels(Constants::getInstance()->getObjWidth()) / 4;
	//***** TRAVEL LATERAL PIXELS AND FIND A GAP
	for (int i = src.cols / 2; i > 0; i--)
	{
		if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL<src.rows){
			pxYL++;
			if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL<src.rows){
				pxYL ++;
				if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL<src.rows){
					pxYL ++;
					if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL < src.rows){
						pxYL -= 4;
						if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL < src.rows){
							pxYL--;
							if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL < src.rows){
								pxYL--;
								if ((int)src.at<uchar>(pxYL, i) == 0 && pxYL>0 && pxYL < src.rows && i < lLimit){
									Point m3 = Point(i, pxYL - 20);
									Point m4 = Point(i, pxYL + 20);
									posL = i;
									line(this->src, m3, m4, Scalar(0, 0, 255), 1, CV_AA);
									break;
								}
								else if (i > lLimit)
									pxYL += 3;
							}
						}
					}
				}
			}
		}
			
	}
	for (int i = src.cols / 2; i < src.cols; i++)
	{
		if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL<src.rows){
			pxYR++;
			if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL<src.rows){
				pxYR ++;
				if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL<src.rows){
					pxYR ++;
					if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL < src.rows){
						pxYR -= 4;
						if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL < src.rows){
							pxYR--;
							if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL < src.rows){
								pxYR--;
								if ((int)src.at<uchar>(pxYR, i) == 0 && pxYL>0 && pxYL<src.rows && i > rLimit){
									Point m3 = Point(i, pxYR - 20);
									Point m4 = Point(i, pxYR + 20);
									posR = i;
									line(this->src, m3, m4, Scalar(0, 0, 255), 1, CV_AA);
									break;
								}
								else if (i < rLimit)
									pxYR += 3;
							}
						}
					}
				}
			}
		}
	}
	Constants::getInstance()->actHorDist = (pxYR - pxYL);
	//***** FIND VERTICAL LINE
	int pyYT = 1;
	int pyYB = 1;
	for (int i = (src.cols / 2)+20; i > 0; i--)
	{
		if ((int)src.at<uchar>(src.rows/2, i) > 0)
		{
			Point m1 = Point(i, imgHeight / 2 - 20);
			Point m2 = Point(i, imgHeight / 2 + 20);
			line(this->src, m1, m2, Scalar(0, 0, 255), 1, CV_AA);
			pyYT = i;
			pyYB = i;
			break;
		}
	}
	//***** TRAVEL VERTICAL PIXELS AND FIND A GAP
	for (int i = src.rows / 2; i >0; i--)
	{
		if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT<src.cols){
			pyYT++;
			if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT<src.cols){
				pyYT++;
				if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT<src.cols){
					pyYT ++;
					if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT < src.cols){
						pyYT -= 4;
						if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT < src.cols){
							pyYT--;
							if ((int)src.at<uchar>(i, pyYT) == 0 && pyYT>0 && pyYT < src.cols){
								pyYT--;
								if ((int)src.at<uchar>(pyYT, i) == 0 && pyYT>0 && pyYT < src.cols){
									Point m3 = Point(pyYT - 20, i);
									Point m4 = Point(pyYT + 20, i);
									posT = i;
									line(this->src, m3, m4, Scalar(0, 0, 255), 1, CV_AA);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = src.rows / 2; i < src.rows; i++)
	{
		if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB<src.cols){
			pyYB++;
			if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB<src.cols){
				pyYB++;
				if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB<src.cols){
					pyYB ++;
					if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB < src.cols){
						pyYB -= 4;
						if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB < src.cols){
							pyYB--;
							if ((int)src.at<uchar>(i, pyYB) == 0 && pyYB>0 && pyYB < src.cols){
								pyYB--;
								if ((int)src.at<uchar>(pyYB, i) == 0 && pyYB>0 && pyYB < src.cols){
									Point m3 = Point(pyYB - 20, i);
									Point m4 = Point(pyYB + 20, i);
									posB = i;
									line(this->src, m3, m4, Scalar(0, 0, 255), 1, CV_AA);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	Constants::getInstance()->actHorDist = (posR - posL);
	this->calculateGripper(posR, posT);
	return(0);
}
int Detection::calibrateMM()
{
	this->calibValue = Constants::getInstance()->actHorDist;
	Constants::getInstance()->setCalibVal(this->calibValue);
	if (this->calibValue < 1)
		return -1;
	return 1;
}
int Detection::pixelsToMm(int px)
{
	if (this->calibValue == 0)
	{
		return(0);
	}
	int x = (210 * px) / this->calibValue;
	return(x);
}
int Detection::mmToPixels(int mm)
{
	if (this->calibValue == 0)
	{
		return(0);
	}
	int x = (mm*this->calibValue) / 210;
}
Point2i Detection::getOffset()
{
	while (sample.size() < sampleSize)
	{
		AfxMessageBox(_T("Yeahh, that's wrong!"));
		Sleep(10);
	}
	int totalX=0;
	int totalY=0;
	Point2i media = Point2i(1,1);
	for (int i = 0; i < sample.size(); i++)
	{
		totalX += sample[i].x;
		totalY += sample[i].y;
	}
	//CString str;
	//str.Format(_T("sample[1]: %d"), totalY);
	//AfxMessageBox(str);
	int calc = totalY / sample.size();
	calc -=imgHeight/2;
	media = Point2i(this->pixelsToMm((totalX/sample.size()-src.cols/2)), this->pixelsToMm(calc));

	__raise enableOut(1);

	this->canRead = false;

	return(media);
}
void Detection::enableRead(){
	sample.clear();
	this->canRead = true;
}
void Detection::calculateGripper(int x, int y)
{
	Point2f center = Point2f(x - (this->mmToPixels(Constants::getInstance()->getObjWidth()) / 2), y + (this->mmToPixels(Constants::getInstance()->getObjHeight()) / 2));
	Rect rect = Rect(0, 0, this->mmToPixels(Constants::getInstance()->getObjWidth()), this->mmToPixels(Constants::getInstance()->getObjHeight()));

	
	this->drawGripper(this->src, center, rect, 0);
}
void Detection::startVision()
{
	this->hasVision = true;
}
//***** DRAW GRIPPER
void Detection::drawGripper(Mat img, Point2i center, Rect rect, float angle)
{
	RotatedRect rRect = RotatedRect(center, Size2f(rect.width, rect.height), angle);
	CString str;
	str.Format(_T("%d"), center.x);
	//AfxMessageBox(str);
	if (sample.size() < sampleSize && this->hasVision)
	{
		sample.push_back(Point2i(center.x, center.y));
		CString str;
		str.Format(_T("%d"), sample[sample.size()-1]);
		//AfxMessageBox(str);
	}

	Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
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
		if (thresholdFunction == 1)
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