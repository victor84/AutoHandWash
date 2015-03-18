#pragma once
#include "stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "AsyncClientSocket.h"
#include "ServerPacketParser.h"
#include "PacketToRawData.h"
#include "DeviceStructuresConverter.h"
#include "SingleServerSocket.h"
#include "DeveiceMessageDescription.h"
#include "DevicePacketConvertor.h"

// �������� ������ ���������
class CTestLogic
{
	tools::logging::CTraceError* _tr_error;

	// �������� ����� ������ � �������
	tools::lock_vector<tools::data_wrappers::_tag_data_const> _received_server_data;

	// �������� ������ � ��������� ����������
	tools::lock_vector<tools::data_wrappers::_tag_data_const> _received_device_data;

	// ��� �������� ������
	tools::networking::CAsyncClientSocket _client_socket;

	// ��� ������ ������� � ���������� ����������
	tools::networking::CSingleServerSocket _device_socket;

	// ������� �����
	std::thread _this_thread;

	bool _continue_work;

	server_exchange::CServerPacketParser _packet_parser;
	server_exchange::CPacketToRawData _packet_to_raw_data;

	logic_structures::CDeviceStructuresConverter _device_structures_converter;

	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_device> _device_packet_from_device_convertor;

	CDeveiceMessageDescription _device_message_descriptor;

	// �����, �������� � ����������
	std::shared_ptr<logic_structures::tag_base_data_from_device> _received_device_packet;

	// ����� ������
	void thread_method();

	// ���������� ��� ����� ������ c �������
	void on_server_data_received(tools::data_wrappers::_tag_data_managed data);

	// ���������� ��� ����� ������ � ��������� ����������
	void on_device_data_received(tools::data_wrappers::_tag_data_managed data);

	bool send_identification_packet();

	bool send_settings_packet();

	bool send_counters_packet();

	bool send_log_record_packet();

	void process_device_packet();

public:
	CTestLogic();
	~CTestLogic();

	bool Start(tools::networking::tag_connection_params server_connection_params, 
			   tools::networking::tag_connection_params device_connection_params);

	void Stop();

};

