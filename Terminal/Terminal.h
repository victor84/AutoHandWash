
// Terminal.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTerminalApp:
// О реализации данного класса см. Terminal.cpp
//

class CTerminalApp : public CWinApp
{
public:
	CTerminalApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTerminalApp theApp;