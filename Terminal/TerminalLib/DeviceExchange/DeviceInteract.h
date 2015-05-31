#pragma once
#include "stdafx.h"
#include "lock_vector.h"
#include "device_protocol.h"
#include "TraceError.h"
#include "data_from_pc.h"
#include "data_from_device.h"
#include "tools_structures.h"
#include "LogicDataFromDeviceToPacketFromDeviceConverter.h"
#include "LogicDataFromPcToPacketFromPcConverter.h"
#include "PacketFromDeviceToLogicDataFromDeviceConverter.h"
#include "SingleServerSocket.h"
#include "CommonSettings.h"
#include "DevicePacketConvertor.h"
#include "DeviceMessageDescription.h"
#include "SerialWrapper.h"

namespace device_exchange
{

/*
CDeviceInteract
�������������� � �����������
*/
class CDeviceInteract
{
	tools::logging::CTraceError* _tr_error;

	// (logic) ������ ��� ���������� �� ������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& _packets_to_device;

	// (logic) ������ ��� ������ �� ����������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& _packets_to_logic;

	std::thread _this_thread;

	tools::e_work_loop_status _work_loop_status;
	tools::e_init_state _init_state;

	// (device)����� �� ���������� ��� ������ -> (logic)����� ��� ������ �� ����������
	// (device)_packets_from_device -> (logic)_packets_to_logic
	logic_structures::CPacketFromDeviceToLogicDataFromDeviceConverter _packet_from_device_to_packet_to_logic;

	// (logic)����� ��� ���������� �� ������ -> (device)����� �� ������ ��� ����������
	// (logic)_packets_to_device -> (device)_packets_from_logic
	logic_structures::CLogicDataFromPcToPacketFromPcConverter _packets_to_device_to_packets_from_logic;

	// ����������� � ����������
	tools::serial_port::CSerialWrapper _device_connection;
	tools::lock_vector<tools::data_wrappers::_tag_data_managed> _device_raw_data;

	// ���������
	const logic_settings::CCommonSettings& _settings_module;

	// ������ ������� �� ����������
	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_device> _device_packet_parser;

	// ��������� ������� ��� ����������
	device_exchange::CDevicePacketConvertor<device_exchange::tag_packet_from_pc> _device_packet_creator;

#ifdef DEBUG
	CDeviceMessageDescription _message_description;
#endif // DEBUG

#pragma region ��������, ��� ���������� �� �����
	// ����� �������� ���������
	uint64_t _send_packet_time;

	// ���������� ��� ������������ �������
	Concurrency::call<int32_t> _on_timer_call;

	// ������ ��������
	Concurrency::timer<int32_t> _timer;

	Concurrency::critical_section _check_device_cs;

	std::function<void(void)> _on_device_not_available_fn;

	// ������ ��������
	void on_timer(uint32_t);

	// ��������� ������� �������� ������ �� ����������
	void set_send_packet_time(time_t t);

	// ��������� ������� �������� ������ �� ����������
	time_t get_send_packet_time();

#pragma endregion


	// ������� ������
	void thread_fn();

	// �������������� ���������� -> ������
	void device_to_logic();

	// �������������� ������ -> ����������
	void logic_to_device();

	tools::e_init_state init();

public:
	CDeviceInteract(const logic_settings::CCommonSettings& settings_module,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& packets_to_logic,
					tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& packets_to_device,
					std::function<void(void)> on_device_not_available_fn);
	virtual ~CDeviceInteract();

	bool Start();

	void Stop();

	bool IsStarted();
};
}