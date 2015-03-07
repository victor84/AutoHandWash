// TerminalEmulator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "TerminalEmulator.h"
#include "SettingsLoader.h"
#include "SingleServerSocket.h"
#include "tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace tools::settings;

CSettingsLoader* settings_loader;
tools::logging::CTraceError* tr_error;

tools::lock_vector<tools::data_wrappers::_tag_data_const> received_data;

tools::networking::CSingleServerSocket server_socket(received_data);

tools::lock_vector<std::wstring> log_messages;

void Initialize();
void PrepareExit();
void show_messages();

#define log(str) { tr_error->trace_message(str); show_messages(); };

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
	::system("pause");
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

	tools::networking::tag_connection_params connection_params;
	connection_params.port = tools::wstring_to_string(settings_loader->get_string(_T("port")).GetString());

	if (tools::networking::e_socket_result::success == server_socket.Start(connection_params))
	{
		log(_T("Сервер запущен"));
	}
	else
	{
		log(_T("Ошибка запуска сервера"));
	}



}

void PrepareExit()
{
	server_socket.Stop();

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





