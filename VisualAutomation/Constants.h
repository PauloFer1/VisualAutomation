#pragma once

class Constants
{
private:
	static bool instanceFlag;
	static Constants *single;
public:
	Constants();
	virtual ~Constants();
	static Constants* getInstance();
private:
	int blur;
	int cannyVal;
	int brightnessVal;
	int exposureVal;
	int constastVal;
	int zoomVal;
	bool hasThreshold;
	int typeThreshold;
	int thresholdVal;
	int objectWidth;
	int objectHeight;
	int calibrationVal;
public:
	void setBlur(int val);
	void setCanny(int val);
	void setBright(int val);
	void setExposure(int val);
	void setConstrast(int val);
	void setZoom(int val);
	void setHasThreshold(bool val);
	void setTypeThresh(int val);
	void setObjWidth(int val);
	void setObjHeight(int val);
	void setCalibVal(float val);
	void setThresholdVal(int val);
	int getBlur();
	int getCanny();
	int getConstrast();
	int getZoom();
	int getExposure();
	int getObjHeight();
	int getObjWidth();
	int getTypeThresh();
	int getBright();
	bool getHasThreshold();
	float getCalibVal();
	int getThresholdVal();
};

