
// SimpleDeviceEmulator.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CSimpleDeviceEmulatorApp:
// � ���������� ������� ������ ��. SimpleDeviceEmulator.cpp
//

class CSimpleDeviceEmulatorApp : public CWinApp
{
public:
	CSimpleDeviceEmulatorApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CSimpleDeviceEmulatorApp theApp;