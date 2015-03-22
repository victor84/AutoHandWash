// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ��������� ����� ������������ ���������� �� ���������� Windows
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ��������� ������������ CString ����� ������

#ifdef _AFXDLL
	#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // ������� ��������� ��������� ���������� MFC � ���������� �����
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ��������� ����� ������������ ���������� �� ���������� Windows
#endif

#include <afx.h>
#include <afxwin.h>         // �������� � ����������� ���������� MFC
#include <afxext.h>         // ���������� MFC
#include <afxmt.h>


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // ��������� MFC ��� ������� ��������� ���������� Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // ��������� MFC ��� ������� ��������� ���������� Windows
#endif // _AFX_NO_AFXCMN_SUPPORT



// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������

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
