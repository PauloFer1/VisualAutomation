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
	m_hCam = 0;
	m_nSizeX = 640;		//rc.right - rc.left;	// set video width  to fit into display window
	m_nSizeY = 480;
	m_nRenderMode = IS_RENDER_FIT_TO_WINDOW;
	OpenCamera();
	/*
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
	*/
	Sleep(5);
	hasCamera = true;
	return 1;
}


int Camera::closeCamera()
{
	if (m_hCam != 0)
	{
		//free old image mem.
		is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
	} 

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
	//cap.set(CV_CAP_PROP_EXPOSURE, value);
	double exp = (this->m_ExposureInc*this->m_MaxExposure) / 100;
	this->m_Exposure = exp*value;
	is_Exposure(this->m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, &m_Exposure, sizeof(m_Exposure));
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

bool Camera::OpenCamera()
{
	if (m_hCam != 0)
	{
		//free old image mem.
		is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
		is_ExitCamera(m_hCam);
	}

	// init camera
	m_hCam = (HIDS)0;						// open next camera
	m_Ret = InitCameraIDS(&m_hCam, NULL);		// init camera - no window handle required
	if (m_Ret == IS_SUCCESS)
	{
		// Get sensor info
		is_GetSensorInfo(m_hCam, &m_sInfo);

		GetMaxImageSize(&m_nSizeX, &m_nSizeY);

		// setup the color depth to the current windows setting
		is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
		m_nColorMode = IS_CM_MONO8;
		m_nBitsPerPixel = 8;
		is_SetColorMode(m_hCam, m_nColorMode);
		// memory initialization
		is_AllocImageMem(m_hCam,
			m_nSizeX,
			m_nSizeY,
			m_nBitsPerPixel,
			&m_pcImageMemory,
			&m_lMemoryId);
		is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);	// set memory active

		// display initialization
		IS_SIZE_2D imageSize;
		imageSize.s32Width = m_nSizeX;
		imageSize.s32Height = m_nSizeY;

		is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, (void*)&imageSize, sizeof(imageSize));

		is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);

		// enable the dialog based error report
		m_Ret = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); //IS_DISABLE_ERR_REP);
		if (m_Ret != IS_SUCCESS)
		{
			AfxMessageBox(TEXT("ERROR: Can not enable the automatic uEye error report!"), MB_ICONEXCLAMATION, 0);
			return false;
		}

	}
	else
	{
		AfxMessageBox(TEXT("ERROR: Can not open uEye camera!"), MB_ICONEXCLAMATION, 0);
		return false;
	}

	return true;
}
INT Camera::InitCameraIDS(HIDS *hCam, HWND hWnd)
{
	INT nRet = is_InitCamera(hCam, hWnd);
	/************************************************************************************************/
	/*                                                                                              */
	/*  If the camera returns with "IS_STARTER_FW_UPLOAD_NEEDED", an upload of a new firmware       */
	/*  is necessary. This upload can take several seconds. We recommend to check the required      */
	/*  time with the function is_GetDuration().                                                    */
	/*                                                                                              */
	/*  In this case, the camera can only be opened if the flag "IS_ALLOW_STARTER_FW_UPLOAD"        */
	/*  is "OR"-ed to m_hCam. This flag allows an automatic upload of the firmware.                 */
	/*                                                                                              */
	/************************************************************************************************/
	if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
	{
		// Time for the firmware upload = 25 seconds by default
		INT nUploadTime = 25000;
		is_GetDuration(*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

		CString Str1, Str2, Str3;
		Str1 = "This camera requires a new firmware. The upload will take about";
		Str2 = "seconds. Please wait ...";
	//	Str3.Format("%s %d %s", Str1, nUploadTime / 1000, Str2);
		AfxMessageBox(Str3, MB_ICONWARNING);

		// Set mouse to hourglass
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

		// Try again to open the camera. This time we allow the automatic upload of the firmware by
		// specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
		*hCam = (HIDS)(((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
		nRet = is_InitCamera(hCam, hWnd);
	}
	CString str;

	is_Exposure(this->m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, &m_Exposure, sizeof(m_Exposure));
	is_Exposure(this->m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_INC, &m_ExposureInc, sizeof(m_ExposureInc));
	is_Exposure(this->m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MAX, &m_MaxExposure, sizeof(m_MaxExposure));
	is_Exposure(this->m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MIN, &m_MinExposure, sizeof(m_MinExposure));

	str.Format(_T("val: %f"), this->m_MinExposure);
	AfxMessageBox(str);

	str.Format(_T("val: %f"), this->m_ExposureInc);
	AfxMessageBox(str);

	str.Format(_T("val: %f"), this->m_MaxExposure);
	AfxMessageBox(str);

	return nRet;
}
void Camera::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{
	// Check if the camera supports an arbitrary AOI
	// Only the ueye xs does not support an arbitrary AOI
	INT nAOISupported = 0;
	BOOL bAOISupported = TRUE;
	if (is_ImageFormat(m_hCam,
		IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
		(void*)&nAOISupported,
		sizeof(nAOISupported)) == IS_SUCCESS)
	{
		bAOISupported = (nAOISupported != 0);
	}

	if (bAOISupported)
	{
		// All other sensors
		// Get maximum image size
		SENSORINFO sInfo;
		is_GetSensorInfo(m_hCam, &sInfo);
		*pnSizeX = sInfo.nMaxWidth;
		*pnSizeY = sInfo.nMaxHeight;
	}
	else
	{
		// Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		*pnSizeX = imageSize.s32Width;
		*pnSizeY = imageSize.s32Height;
	}
}

void Camera::captureImg()
{
	if (m_hCam == 0)
		OpenCamera();

	if (m_hCam != 0)
	{
		//if (is_FreezeVideo(m_hCam, IS_WAIT) == IS_SUCCESS)
		//	is_RenderBitmap(m_hCam, m_lMemoryId, m_hWndDisplay, m_nRenderMode);
		is_FreezeVideo(m_hCam, IS_WAIT);

		if (m_pcImageMemory)
		{
			is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
			m_pcImageMemory = NULL;
		}

		if (is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId) != IS_SUCCESS)
		{
			AfxMessageBox(TEXT("Memory allocation failed!"), MB_ICONWARNING);
		}
		else
		{
			is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
		}
	}
}
HWND Camera::getWndDisplay()
{
	return(m_hWndDisplay);
}
void Camera::setWndDisplay(CStatic * wndD)
{
//	this->m_hWndDisplay = G
}
char* Camera::getFrame()
{
	if (m_hCam == 0)
		OpenCamera();

	if (m_pcImageMemory)
	{
		is_FreeImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
		m_pcImageMemory = NULL;
	}
	is_AllocImageMem(m_hCam, m_nSizeX, m_nSizeY, m_nBitsPerPixel, &m_pcImageMemory, &m_lMemoryId);
	is_SetImageMem(m_hCam, m_pcImageMemory, m_lMemoryId);
	is_FreezeVideo(m_hCam, IS_WAIT);
	//char *dest;
	//is_CopyImageMem(m_hCam, m_pcImageMemory, m_lMemoryId, dest);
	//this->captureImg();
	return(m_pcImageMemory);
}
int Camera::getCameraWidth(){
	SENSORINFO sInfo;
	is_GetSensorInfo(m_hCam, &sInfo);
	return (int)(sInfo.nMaxWidth);
}
int Camera::getCameraHeight(){
	SENSORINFO sInfo;
	is_GetSensorInfo(m_hCam, &sInfo);
	return (int)(sInfo.nMaxHeight);
}
bool Camera::isOpen()
{
	if (m_hCam == 0)
		return(false);
	return true;
}