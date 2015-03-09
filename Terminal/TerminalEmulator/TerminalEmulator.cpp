// TerminalEmulator.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "TerminalEmulator.h"
#include "SettingsLoader.h"
#include "SingleServerSocket.h"
#include "tools.h"
#include "DeveiceMessageDescription.h"
#include "DevicePacketConvertor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;

using namespace tools::settings;
using namespace device_exchange;

CSettingsLoader* settings_loader;
tools::logging::CTraceError* tr_error;

tools::lock_vector<tools::data_wrappers::_tag_data_const> received_data;

void on_data_received(tools::data_wrappers::_tag_data_managed data);

tools::networking::CSingleServerSocket server_socket(received_data, &on_data_received);

tools::lock_vector<std::wstring> log_messages;

CDeveiceMessageDescription _device_message_descriptor;

CDevicePacketConvertor<tag_packet_from_device> _device_packet_convertor;

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
		LOG(_T("\r\n������ �������\r\n"));
	}
	else
	{
		LOG(_T("\r\n������ ������� �������\r\n"));
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

void on_data_received(tools::data_wrappers::_tag_data_managed data)
{
	tag_packet_from_device packet;

	_device_packet_convertor.Parse(data, packet);

	std::wstring describe_text = _device_message_descriptor.describe_message(packet);

	LOG(describe_text);
}




