
// SimpleDeviceEmulator.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CSimpleDeviceEmulatorApp:
// О реализации данного класса см. SimpleDeviceEmulator.cpp
//

class CSimpleDeviceEmulatorApp : public CWinApp
{
public:
	CSimpleDeviceEmulatorApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CSimpleDeviceEmulatorApp theApp;