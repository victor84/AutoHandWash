// TerminalEmulator.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "TerminalEmulator.h"
#include "SettingsLoader.h"
#include "SingleServerSocket.h"
#include "tools.h"
#include "DeviceMessageDescription.h"
#include "DevicePacketConvertor.h"
#include "AsyncClientSocket.h"
#include "TestLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace tools::settings;
using namespace device_exchange;

CSettingsLoader* settings_loader;
tools::logging::CTraceError* tr_error;

tools::lock_vector<std::wstring> log_messages;

CDevicePacketConvertor<tag_packet_from_device> _device_packet_convertor;

CTestLogic test_logic;

void Initialize();
void PrepareExit();
void show_messages();

#define LOG(str) { tr_error->trace_message(str); show_messages(); };

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

	Initialize();

	std::wcout << _T("Enter для выхода...") << std::endl;

	while (true)
	{
		show_messages();

		::Sleep(1000);

		if (0 != ::GetAsyncKeyState(VK_RETURN))
		{
			std::wcout << _T("Подготовка к выходу...") << std::endl;
			break;
		}
	}

	PrepareExit();
	return nRetCode;
}

void Initialize()
{
	setlocale(LC_ALL, "Russian");

	tr_error = tools::logging::CTraceError::get_instance();
	tr_error->set_messages_storages(&log_messages, nullptr);
	settings_loader = CSettingsLoader::get_instance();

	settings_loader->add_parameter(CSettingsLoader::e_parameter_data_type::type_string, _T("device"), _T("port"));

	settings_loader->read_all();

	tools::networking::tag_connection_params server_connection_params;
	server_connection_params.port = "13000";
	server_connection_params.address = "127.0.0.1";
	server_connection_params.reconnection_timeout = 10000;

	tools::networking::tag_connection_params device_connection_params;
	device_connection_params.port = tools::wstring_to_string(settings_loader->get_string(_T("port"), _T("device")).GetString());


	test_logic.Start(server_connection_params, device_connection_params);
}

void PrepareExit()
{
	test_logic.Stop();

	delete settings_loader;
	delete tr_error;
}

void show_messages()
{
	for (std::wstring message : log_messages.get_with_cleanup())
	{
		std::wcout << message << std::endl;
	}
}



