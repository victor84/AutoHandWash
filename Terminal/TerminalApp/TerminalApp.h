
// TerminalApp.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTerminalAppApp:
// О реализации данного класса см. TerminalApp.cpp
//

class CTerminalAppApp : public CWinApp
{
public:
	CTerminalAppApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTerminalAppApp theApp;