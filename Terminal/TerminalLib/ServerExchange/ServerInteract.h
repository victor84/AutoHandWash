#pragma once
#include "stdafx.h"
#include "TraceError.h"
#include "AsyncClientSocket.h"
#include "ServerPacketParser.h"
#include "PacketToRawData.h"
#include "CommonSettings.h"
#include "server_structures.h"

namespace server_exchange
{
/*
CServerInteract
����� ������� � ��������
*/
class CServerInteract
{
	tools::logging::CTraceError* _tr_error;

	// ����� ������ � �������
	tools::lock_vector<tools::data_wrappers::_tag_data_const> _raw_data_from_server;

	// ���������� � ��������
	tools::networking::CAsyncClientSocket _client_socket;

	// ������ ������� �� �������
	server_exchange::CServerPacketParser _parser;

	// ��������������� ������� � ����� ������
	server_exchange::CPacketToRawData _packet_to_raw_data;

	// ������ ��� ������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& _packets_to_logic;

	// ������ ��� �������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& _packets_to_server;

	// �������� �������������
	tools::e_init_state _init_state;

	// ��������� �������� ������
	tools::e_work_loop_status _work_loop_status;

	// ������� �����
	std::thread _this_thread;

	// ���������
	logic_settings::CCommonSettings& _settings_module;

	// ����� ������
	void thread_fn();

	bool parse_from_server();

	bool parse_transport_packet(const tag_transport_packet transport_packet);

	bool send_to_server();

public:
	CServerInteract(logic_settings::CCommonSettings& settings_module,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_logic,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>>& packets_to_server);
	virtual ~CServerInteract();

	bool Start();

	void Stop();

};
}


