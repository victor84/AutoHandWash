#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"
#include "DeviceInteract.h"
#include "ServerInteract.h"
#include "state.h"
#include "ILogic.h"

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
class CLogic : CLogicAbstract, public ILogic
{
	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

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

	// ���������� ��� ��������� ����� � �������
	std::function<void(int16_t, int16_t)> _on_change_time_and_money_fn;

	// ���������� ��� ��������� ���������
	std::function<void(e_service_name, std::wstring)> _on_service_changed_fn;

	// ���������� ��� ��������� ���������
	std::function<void(e_state)> _on_state_changed_fn;

	// ���������� ��� ���������� �����
	std::function<void(uint16_t)> _on_cache_refilled;

	// ���������� ����� ��������� �������� �����
	std::function<void(std::vector<tag_service_info>)> _on_service_info_readed;

	// ��������� ���������
	void fill_states();

	// ������� �������� ������
	void thread_fn();

	// �������������
	tools::e_init_state init();

	// �������� ���������� ���������� �� �������
	void send_services_info();

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

	virtual void open_valve(byte number) final;

	virtual void read_eeprom(byte cell_number) final;

	virtual void write_eeprom(byte cell_number, uint32_t value) final;

	virtual void time_and_money(int16_t time, int16_t money) final;

	virtual void close_valve(byte number) final;

	// ������ � ������
	virtual bool Start() final;

	// ���������
	virtual void Stop() final;

	virtual void SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t) > fn) final;

	virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring) > fn) final;

	virtual void SetOnStateChangedFn(std::function<void(e_state)> fn) final;

	virtual void SetOnCacheRefilledFn(std::function<void(uint16_t) > fn) final;

	virtual void SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>) > fn) final;

public:
	CLogic();
	~CLogic();



};
}



