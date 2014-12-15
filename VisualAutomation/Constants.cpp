#include "stdafx.h"
#include "Constants.h"


Constants::Constants()
{
}

Constants::~Constants()
{
}

bool Constants::instanceFlag = false;
Constants* Constants::single = NULL;

Constants* Constants::getInstance(){
	if (!instanceFlag)
	{
		single = new Constants();
		instanceFlag = true;
		return(single);
	}
	else
		return(single);
}

void Constants::setBlur(int val){
	this->blur = val;
}
void Constants::setBright(int val){
	this->brightnessVal = val;
}
void Constants::setCanny(int val){
	this->cannyVal = val;
}
void Constants::setConstrast(int val){
	this->constastVal = val;
}
void Constants::setZoom(int val){
	this->zoomVal = val;
}
void Constants::setExposure(int val){
	this->exposureVal = val;
}
void Constants::setObjHeight(int val){
	this->objectHeight = val;
}
void Constants::setObjWidth(int val){
	this->objectWidth = val;
}
void Constants::setTypeThresh(int val){
	this->typeThreshold = val;
}
void Constants::setHasThreshold(bool val){
	this->hasThreshold = val;
}
void Constants::setCalibVal(int val){
	this->calibrationVal = val;
}
void Constants::setThresholdVal(int val){
	this->thresholdVal = val;
}
int Constants::getCalibVal(){
	return(this->calibrationVal);
}
int Constants::getThresholdVal(){
	return(this->thresholdVal);
}
int Constants::getBlur()
{
	return this->blur;
}


int Constants::getCanny()
{
	return this->cannyVal;
}


int Constants::getConstrast()
{
	return this->constastVal;
}


int Constants::getZoom()
{
	return this->zoomVal;
}


int Constants::getExposure()
{
	return this->exposureVal;
}


int Constants::getObjHeight()
{
	return this->objectHeight;
}


int Constants::getObjWidth()
{
	return this->objectWidth;
}


int Constants::getTypeThresh()
{
	return this->typeThreshold;
}


int Constants::getBright()
{
	return this->brightnessVal;
}


bool Constants::getHasThreshold()
{
	return this->hasThreshold;
}
int Constants::getHasThresholdInt()
{
	int has = 0;
	if (this->hasThreshold)
		has = 1;

	return has;
}
