#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"
#include "DeviceInteract.h"
#include "ServerInteract.h"
#include "state.h"

/*!
 * \file Logic.h
 *
 * \author Victor Mulika
 * \date 21 ���� 2015
 *
 * 
 */

namespace logic
{
/*
CLogic
������ ������ ���������
*/
class CLogic : CLogicAbstract
{
	tools::logging::CTraceError* _tr_error;

	tools::e_work_loop_status _work_loop_status;

	tools::e_init_state _init_state;

	// ������ ������
	std::thread _this_thread;

	// ������ ����� �������
	logic_settings::CCommonSettings _common_settings;

	// ������ ������������ �����, ������ � ��������
	logic_settings::CCorrespondSettings _correspond_settings;

	// ������ ��� ����������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>> _packets_to_device;

	// ������ �� ����������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>> _packets_from_device;

	// ������ �� �������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_from_server;

	// ������ ��� �������
	tools::lock_vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> _packets_to_server;

	// ������ ������ � �����������
	device_exchange::CDeviceInteract _device_interact;

	// ������ ������ ������ � ��������
	server_exchange::CServerInteract _server_interact;

	// ���������
	std::map<e_state, std::shared_ptr<IState>> _states;

	// ������� ���������
	std::shared_ptr<IState> _current_state;

	// ��������� ���������
	void fill_states();

	// ������� �������� ������
	void thread_fn();

	// �������������
	tools::e_init_state init();

	// ���������� � ��������
	void on_connected_to_server();

	// ���������� �� �������
	void on_disconnected_from_derver();

	// �������� ������������������ ������
	void send_identification_packet();

	// �������� ������ � �����������
	void send_settings_packet();

	virtual std::shared_ptr<IState> get_state(e_state state) final;

	virtual void set_state(e_state state) final;

	// ������ � ��������� ��������� �� ����������
	void process_messages_from_device();

	// ������� ��������� �� ����������
	void process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message);

	template<typename _MessageType>
	_MessageType* get_device_message_pointer(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
	{
		return dynamic_cast<_MessageType*>(message.get());
	}

public:
	CLogic();
	~CLogic();

	// ������ � ������
	bool Start();

	// ���������
	void Stop();

	virtual void open_valve(byte number) final;

	virtual void read_eeprom(byte cell_number) final;

	virtual void write_eeprom(byte cell_number, uint32_t value) final;

};
}



