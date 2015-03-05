// TerminalEmulator.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "TerminalEmulator.h"
#include "SettingsLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;

using namespace tools::settings;

CSettingsLoader* settings_loader;
tools::logging::CTraceError* tr_error;


void Initialize();
void PrepareExit();

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

	Initialize();

	PrepareExit();
	return nRetCode;
}

void Initialize()
{
	setlocale(LC_ALL, "Russian");

	tr_error = tools::logging::CTraceError::get_instance();
	settings_loader = CSettingsLoader::get_instance();

	settings_loader->add_parameter(CSettingsLoader::e_parameter_data_type::type_string, _T("device"), _T("host"));
	settings_loader->add_parameter(CSettingsLoader::e_parameter_data_type::type_string, _T("device"), _T("port"));

	settings_loader->read_all();






}

void PrepareExit()
{
	delete settings_loader;
	delete tr_error;


}
