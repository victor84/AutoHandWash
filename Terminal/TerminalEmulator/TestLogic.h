#pragma once
#include "stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "AsyncClientSocket.h"
#include "ServerPacketParser.h"
#include "PacketToRawData.h"

// �������� ������ ���������
class CTestLogic
{
	tools::logging::CTraceError* _tr_error;

	// �������� ����� ������
	tools::lock_vector<tools::data_wrappers::_tag_data_const> _received_data;

	// ��� �������� ������
	tools::networking::CAsyncClientSocket _socket;

	// ������� �����
	std::thread _this_thread;

	bool _continue_work;

	server_exchange::CServerPacketParser _packet_parser;
	server_exchange::CPacketToRawData _packet_to_raw_data;

	// ����� ������
	void thread_method();

	// ���������� ��� ����� ������ �� �������
	void on_data_received(tools::data_wrappers::_tag_data_managed data);

	bool send_identification_packet();

	bool send_settings_packet();

	bool send_counters_packet();

	bool send_log_record_packet();

public:
	CTestLogic();
	~CTestLogic();

	bool Start(tools::networking::tag_connection_params connection_params);

	void Stop();

};

