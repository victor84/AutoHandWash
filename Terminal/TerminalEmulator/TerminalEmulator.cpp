// TerminalEmulator.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "TerminalEmulator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;






int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	UNREFERENCED_PARAMETER(envp);

	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule == NULL)
	{
		// TODO: �������� ��� ������ �������������� ����� ������������
		_tprintf(_T("����������� ������: ��������� ���������� GetModuleHandle\n"));
		nRetCode = 1;
		return nRetCode;
	}
	// ���������������� MFC, � ����� ������ � ��������� �� ������� ��� ����
	if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
	{
		// TODO: �������� ��� ������ �������������� ����� ������������
		_tprintf(_T("����������� ������: ���� ��� ������������� MFC\n"));
		nRetCode = 1;
		return nRetCode;
	}



	return nRetCode;
}

void Initialize()
{

}

void PrepareExit()
{

}
