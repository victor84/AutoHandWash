
// Terminal.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTerminalApp:
// � ���������� ������� ������ ��. Terminal.cpp
//

class CTerminalApp : public CWinApp
{
public:
	CTerminalApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTerminalApp theApp;