// TerminalTest.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "TerminalTest.h"
#include "PacketToRawData.h"
#include "ServerPacketParser.h"
#include "device_protocol.h"
#include "DevicePacketConvertor.h"
#include "SingleServerSocket.h"
#include "AsyncClientSocket.h"
#include "SerialUtils.h"
#include "SerialWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;
using namespace device_exchange;

tools::logging::CTraceError* tr_error;

void TestClient(tools::networking::tag_connection_params connection_params,
				tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data);

void TestServer(tools::networking::tag_connection_params connection_params,
				tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data);

void TestCom(tools::serial_port::tag_connection_params connection_params,
				tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data);

void TestServerMessages();

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

	tr_error = tools::logging::CTraceError::get_instance();

	// TODO: Вставьте сюда код для приложения.
	TestServerMessages();




	tools::lock_vector<tools::data_wrappers::_tag_data_managed> received_data;
	tools::networking::tag_connection_params connection_params;
	connection_params.address = "127.0.0.1";
	connection_params.port = "26000";

	tools::serial_port::tag_connection_params com_params;
	com_params.port_name = _T("COM3");

	TestCom(com_params, received_data);
	// TestServer(connection_params, received_data);
	// TestClient(connection_params, received_data);

	delete tr_error;

	return nRetCode;
}

void TestClient(tools::networking::tag_connection_params connection_params,
				tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data)
{
	tools::networking::CAsyncClientSocket client(received_data, nullptr, nullptr, nullptr);

	client.OpenConnection(connection_params);

	::system("pause");

	client.CloseConnection();
	tr_error->trace_error(_T("Соединение клиента закрыто"));

	::system("pause");
}

void TestServer(tools::networking::tag_connection_params connection_params, 
				tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data)
{
	tools::networking::CSingleServerSocket server(received_data, nullptr);


	server.Start(connection_params);

	::system("pause");

	server.Stop();

	tr_error->trace_error(_T("Сервер остановлен"));

	::system("pause");
}

void TestCom(tools::serial_port::tag_connection_params connection_params, tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data)
{
	tools::serial_port::CSerialWrapper serial_port(received_data, nullptr);

	serial_port.Start(connection_params);

	::system("pause");

	serial_port.Stop();
	tr_error->trace_message(_T("COM-порт остановлен"));

	::system("pause");
}

void TestDeviceMessages()
{
	CDevicePacketConvertor<tag_packet_from_device> device_packet_convertor;

	tag_packet_from_device command_from_device;

	tools::data_wrappers::_tag_data_managed out_data;

	command_from_device.command = e_command_from_device::button_press;
	command_from_device.data0 = 0x12;
	command_from_device.data1 = 0x34;
	command_from_device.data2 = 0x56;
	command_from_device.data3 = 0x78;
	command_from_device.data4 = 0x90;
	command_from_device.data5 = 0x0a;
	command_from_device.crc = device_packet_convertor.CalcCheckSum(command_from_device);

	device_packet_convertor.CreateRawData(command_from_device, out_data);

	ZeroMemory(&command_from_device, sizeof(command_from_device));

	device_packet_convertor.Parse(out_data, command_from_device);
}

void TestServerMessages()
{
	server_exchange::CPacketToRawData pack_to_raw;
	server_exchange::tag_transport_packet transport_packet;
	server_exchange::tag_counters_packet data;
	data.against_midges = 1;
	data.air = 2;
	data.current_cache = 100;
	data.foam = 3;
	data.pressurized_water = 4;
	data.state = logic::e_terminal_state::work;
	data.total_cache = 1000;
	data.vacuum_cleaner = 5;
	data.water_without_pressure = 6;
	data.wax = 7;

	transport_packet.begin = server_exchange::begin_bytes;
	transport_packet.type = server_exchange::e_packet_type::id;
	transport_packet.length = sizeof(data);

	pack_to_raw.CreateCountersPacketRawData(data, transport_packet.data);

	tools::data_wrappers::_tag_data_managed out_data;

	pack_to_raw.CreateRawData(transport_packet, out_data);

	server_exchange::CServerPacketParser parser;
	std::vector<server_exchange::tag_transport_packet> new_transport_packets;
	server_exchange::tag_counters_packet counters_packet;

	parser.ParseTransportPacket(out_data, new_transport_packets);

	parser.ParseCountersPacket(new_transport_packets[0], counters_packet);

}