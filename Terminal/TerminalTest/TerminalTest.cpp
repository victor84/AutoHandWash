// TerminalTest.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "TerminalTest.h"
#include "..\TerminalLib\ServerExchange\PacketToRawData.h"
#include "..\TerminalLib\ServerExchange\PacketParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������������ ������ ����������

CWinApp theApp;

using namespace std;

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

	server_exchange::CPacketParser parser;
	std::vector<server_exchange::tag_transport_packet> new_transport_packets;
	parser.ParseTransportPacket(out_data, new_transport_packets);

	return nRetCode;
}