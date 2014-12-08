#pragma once
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#import "msxml6.dll" raw_interfaces_only
#include "Constants.h"

class XMLLoader
{
private:
	static bool instanceFlag;
	static XMLLoader *single;

private:
	HRESULT VariantFromString(PCWSTR wszValue, VARIANT &variant);
	HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc);
	HRESULT ReportParseError(IXMLDOMDocument *pDoc, char *szDesc);

public:
	XMLLoader();
	virtual ~XMLLoader();
	static XMLLoader* getInstance();
	void saveXML();
	void loadDOM();

};

