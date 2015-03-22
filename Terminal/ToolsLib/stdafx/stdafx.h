// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // некоторые конструкторы CString будут явными

#ifdef _AFXDLL
	#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // удалить поддержку элементов управления MFC в диалоговых окнах
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Исключите редко используемые компоненты из заголовков Windows
#endif

#include <afx.h>
#include <afxwin.h>         // основные и стандартные компоненты MFC
#include <afxext.h>         // Расширения MFC
#include <afxmt.h>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // поддержка MFC для типовых элементов управления Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // поддержка MFC для типовых элементов управления Windows
#endif // _AFX_NO_AFXCMN_SUPPORT



// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы

// stl
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <locale>
#include <thread>

// strings
#include <tchar.h>

// windows
#include <ppl.h>
#include <agents.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// com
#include <comdef.h>

// std
#include <stdlib.h>
