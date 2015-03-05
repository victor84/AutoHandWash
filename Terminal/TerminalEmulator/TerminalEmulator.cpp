// TerminalEmulator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "TerminalEmulator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

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
		// TODO: Измените код ошибки соответственно своим потребностям
		_tprintf(_T("Критическая ошибка: неудачное завершение GetModuleHandle\n"));
		nRetCode = 1;
		return nRetCode;
	}
	// инициализировать MFC, а также печать и сообщения об ошибках про сбое
	if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
	{
		// TODO: измените код ошибки соответственно своим потребностям
		_tprintf(_T("Критическая ошибка: сбой при инициализации MFC\n"));
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
