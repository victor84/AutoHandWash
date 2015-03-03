// TerminalTest.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "TerminalTest.h"
#include "PacketToRawData.h"
#include "ServerPacketParser.h"
#include "device_protocol.h"
#include "DevicePacketConvertor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;

using namespace std;
using namespace device_exchange;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
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

	// TODO: �������� ���� ��� ��� ����������.
	server_exchange::CPacketToRawData pack_to_raw;
	server_exchange::tag_transport_packet transport_packet;
	uint32_t data = 0xffff;

	transport_packet.begin = server_exchange::begin_bytes;
	transport_packet.type = server_exchange::e_packet_type::id;
	transport_packet.length = sizeof(uint32_t);
	transport_packet.data.copy_data_inside(&data, sizeof(uint32_t));

	tools::data_wrappers::_tag_data_managed out_data;

	pack_to_raw.CreateRawData(transport_packet, out_data);

	server_exchange::CServerPacketParser parser;
	std::vector<server_exchange::tag_transport_packet> new_transport_packets;
	parser.ParseTransportPacket(out_data, new_transport_packets);

	CDevicePacketConvertor<tag_packet_from_device> device_packet_convertor;

	tag_packet_from_device command_from_device;

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


	return nRetCode;
}
