
// couting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcoutingApp:
// �йش����ʵ�֣������ couting.cpp
//

class CcoutingApp : public CWinApp
{
public:
	CcoutingApp();

// ��д
public:
	virtual BOOL InitInstance();
	int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcoutingApp theApp;