#include "stdafx.h"
#include "XMLLoader.h"

using namespace std;

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

XMLLoader::XMLLoader()
{
}


XMLLoader::~XMLLoader()
{
}

bool XMLLoader::instanceFlag = false;
XMLLoader* XMLLoader::single = NULL;

XMLLoader* XMLLoader::getInstance(){
	if (!instanceFlag)
	{
		single = new XMLLoader();
		instanceFlag = true;
		return(single);
	}
	else
		return(single);
}

void XMLLoader::loadDOM()
{
	HRESULT hr = S_OK;
	IXMLDOMDocument *pXMLDom = NULL;
	IXMLDOMParseError *pXMLErr = NULL;
	IXMLDOMNodeList *pNodes = NULL;
	IXMLDOMNode *pNode = NULL;

	BSTR bstrXML = NULL;
	BSTR bstrErr = NULL;
	VARIANT_BOOL varStatus;
	VARIANT varFileName;
	VariantInit(&varFileName);

	CHK_HR(CreateAndInitDOM(&pXMLDom));

	// XML file name to load
	CHK_HR(VariantFromString(L"config.xml", varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus == VARIANT_TRUE)
	{
		CHK_HR(pXMLDom->get_xml(&bstrXML));
		printf("XML DOM loaded from config.xml:\n%S\n", bstrXML);
	}
	else
	{
		// Failed to load xml, get last parsing error
		CHK_HR(pXMLDom->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		printf("Failed to load DOM from config.xml. %S\n", bstrErr);
	}

	// Query a single node.
	BSTR bstrQuery1 = NULL;
	BSTR bstrQuery2 = NULL;
	BSTR bstrNodeName = NULL;
	BSTR bstrNodeValue = NULL;

	bstrQuery1 = SysAllocString(L"//config[1]/*");
	CHK_ALLOC(bstrQuery1);
	CHK_HR(pXMLDom->selectNodes(bstrQuery1, &pNodes));
	if (pNodes)
	{
		/*	printf("Result from selectSingleNode:\n");
		CHK_HR(pNode->get_nodeName(&bstrNodeName));
		printf("Node, <%S>:\n", bstrNodeName);
		SysFreeString(bstrNodeName);
		*/
		long length;
		CHK_HR(pNodes->get_length(&length));
		for (long i = 0; i < length; i++)
		{
			CHK_HR(pNodes->get_item(i, &pNode));
			CHK_HR(pNode->get_nodeName(&bstrNodeName));

			BSTR s;
			char *sc;
			int v;
			if (!wcscmp(bstrNodeName, L"bright"))
			{

				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setBright(v);
			}
			else if (!wcscmp(bstrNodeName, L"contrast"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setConstrast(v);
			}
			else if (!wcscmp(bstrNodeName, L"threshold"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setThresholdVal(v);
			}
			else if (!wcscmp(bstrNodeName, L"blur"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setBlur(v);
			}
			else if (!wcscmp(bstrNodeName, L"exposure"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setExposure(v);
			}
			else if (!wcscmp(bstrNodeName, L"calibValue"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = stof(sc);
				Constants::getInstance()->setCalibVal(v);
			}
			else if (!wcscmp(bstrNodeName, L"objectWidth"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setObjWidth(v);
			}
			else if (!wcscmp(bstrNodeName, L"objectHeight"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setObjHeight(v);
			}
			else if (!wcscmp(bstrNodeName, L"zoom"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setZoom(v);
			}
			else if (!wcscmp(bstrNodeName, L"canny"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setCanny(v);
			}
			else if (!wcscmp(bstrNodeName, L"hasThreshold"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				bool t = true;
				if (sc == "false")
					t = false;
				Constants::getInstance()->setHasThreshold(t);
			}
			else if (!wcscmp(bstrNodeName, L"typeThreshold"))
			{
				pNode->get_text(&s);
				sc = _com_util::ConvertBSTRToString(s);
				v = atoi(sc);
				Constants::getInstance()->setTypeThresh(v);
			}

			SysFreeString(bstrNodeName);
			CHK_HR(pNode->get_xml(&bstrNodeValue));
			SysFreeString(bstrNodeValue);
			SAFE_RELEASE(pNode);
		}
	}
	else
	{
		CHK_HR(ReportParseError(pXMLDom, "Error while calling selectSingleNode."));
	}


CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrXML);
	SysFreeString(bstrErr);
	VariantClear(&varFileName);
}
void XMLLoader::saveXML()
{
	ofstream xml("config.xml");
	if (xml.is_open())
	{
		xml << "<?xml";
		xml << "version = ";
		xml << "'1.0' ?> \n";

		xml << "<config xmlns:dt='urn:schemas - microsoft - com : datatypes'>\n";
		xml << "<canny dt:dt='int'>" << Constants::getInstance()->getCanny() << "</canny>\n";
		xml << "<blur dt:dt='int'>" << Constants::getInstance()->getBlur() << "</blur>\n";
		xml << "<bright dt:dt='int'>" << Constants::getInstance()->getBright() << "</bright>\n";
		xml << "<contrast dt:dt='int'>" << Constants::getInstance()->getConstrast() << "</contrast>\n";
		xml << "<exposure dt:dt='int'>" << Constants::getInstance()->getExposure() << "</exposure>\n";
		xml << "<calibValue dt:dt='float'>" << Constants::getInstance()->getCalibVal() << "</calibValue>\n";
		xml << "<objectWidth dt:dt='int'>" << Constants::getInstance()->getObjWidth() << "</objectWidth>";
		xml << "<objectHeight dt : dt = 'int'>" << Constants::getInstance()->getObjHeight() << "< / objectHeight>";
		xml << "<threshold dt:dt='int'>" << Constants::getInstance()->getThresholdVal() << "</threshold>";
		xml << "<hasThreshold dt : dt = 'bool'>" << Constants::getInstance()->getHasThreshold() << "< / hasThreshold>";
		xml << "<typeThreshold dt : dt = 'int'>" << Constants::getInstance()->getTypeThresh() << "< / typeThreshold>";
		xml << "</config>";

		xml.close();
	}
	cout << "CONFIG FILE SAVED!";
}

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT XMLLoader::VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
	HRESULT hr = S_OK;
	BSTR bstr = SysAllocString(wszValue);
	CHK_ALLOC(bstr);

	V_VT(&Variant) = VT_BSTR;
	V_BSTR(&Variant) = bstr;

CleanUp:
	return hr;
}

// Helper function to create a DOM instance. 
HRESULT XMLLoader::CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
	HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
	if (SUCCEEDED(hr))
	{
		// these methods should not fail so don't inspect result
		(*ppDoc)->put_async(VARIANT_FALSE);
		(*ppDoc)->put_validateOnParse(VARIANT_FALSE);
		(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
	}
	return hr;
}
HRESULT XMLLoader::ReportParseError(IXMLDOMDocument *pDoc, char *szDesc)
{
	HRESULT hr = S_OK;
	HRESULT hrRet = E_FAIL; // Default error code if failed to get from parse error.
	IXMLDOMParseError *pXMLErr = NULL;
	BSTR bstrReason = NULL;

	CHK_HR(pDoc->get_parseError(&pXMLErr));
	CHK_HR(pXMLErr->get_errorCode(&hrRet));
	CHK_HR(pXMLErr->get_reason(&bstrReason));
	printf("%s\n%S\n", szDesc, bstrReason);

CleanUp:
	SAFE_RELEASE(pXMLErr);
	SysFreeString(bstrReason);
	return hrRet;
}

