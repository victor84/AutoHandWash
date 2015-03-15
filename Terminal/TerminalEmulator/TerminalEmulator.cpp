// TerminalEmulator.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "TerminalEmulator.h"
#include "SettingsLoader.h"
#include "SingleServerSocket.h"
#include "tools.h"
#include "DeveiceMessageDescription.h"
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

tools::lock_vector<tools::data_wrappers::_tag_data_const> received_device_data;

void on_device_data_received(tools::data_wrappers::_tag_data_managed data);

tools::networking::CSingleServerSocket server_socket(received_device_data, &on_device_data_received);

tools::lock_vector<std::wstring> log_messages;

CDeveiceMessageDescription _device_message_descriptor;
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
		LOG(_T("\r\nСервер запущен\r\n"));
	}
	else
	{
		LOG(_T("\r\nОшибка запуска сервера\r\n"));
	}

	connection_params.port = "13000";
	connection_params.address = "127.0.0.1";
	connection_params.reconnection_timeout = 10000;

	test_logic.Start(connection_params);
}

void PrepareExit()
{
	server_socket.Stop();
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

void on_device_data_received(tools::data_wrappers::_tag_data_managed data)
{
	tag_packet_from_device packet;

	_device_packet_convertor.Parse(data, packet);

	std::wstring describe_text = _device_message_descriptor.describe_message(packet);

	LOG(describe_text);
}

void on_server_data_received(tools::data_wrappers::_tag_data_managed data)
{

}



