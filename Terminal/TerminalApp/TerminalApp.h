
// TerminalApp.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTerminalAppApp:
// � ���������� ������� ������ ��. TerminalApp.cpp
//

class CTerminalAppApp : public CWinApp
{
public:
	CTerminalAppApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTerminalAppApp theApp;