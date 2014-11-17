#pragma once
class VisualRecognition
{
public:
	VisualRecognition();
	~VisualRecognition();
	int showWnd(CImage *m_img, CStatic s_img);
	void destroyWnd();
	static void __cdecl render(void * parg);
};

