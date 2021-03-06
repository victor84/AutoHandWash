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

	// ��������� ����������, ���������� � ������� � ������� ����� �������� � ����������
	server_exchange::tag_settings_packet _settings_from_server;

	// ����, ��� �������� ��������� � ������� � �� ����� ��������
	bool _need_update_device_settings;

	// ����, ������� ���������� ������
	server_exchange::tag_prize_packet _prize_packet;

	// ����, ��� ����� ������ ����
	bool _need_distribute_prize;

	// ���������� ��� ��������� ����� � �������
	std::function<void(int16_t, int16_t)> _on_change_time_and_money_fn;

	// ���������� ��� ��������� ���������
	std::function<void(e_service_name, std::wstring)> _on_service_changed_fn;

	// ���������� ��� ��������� ���������
	std::function<void(e_state)> _on_state_changed_fn;

	// ���������� ����� ��������� �������� �����
	std::function<void(std::vector<tag_service_info>)> _on_service_info_readed;

	// ���������� ��� ������ ����� (����� �������, ������� ������� ������ ��������)
	std::function<void(int16_t, byte)> _on_distribute_prize;

	// ���������� ��� ���������� �������
	std::function<void(void)> _on_empty_hopper;

	// ���������� ��� ������ �������
	std::function<void(void)> _on_show_advertising;

	// ���������� ��� ��������� ��������� ���������
	std::function<void(logic::e_terminal_state)> _on_terminal_state_changed;

	// ���������� ��� ������ ���������
	std::function<void(std::vector<logic::tag_service_counter>)> _on_show_counters;

	// ����, ��� ���������� ����������
	volatile bool _device_not_available;

	// ����, ��� �� ��������� ��������� ��������� �������� ���� ����������
	volatile bool _terminal_state_changed_sended;

	// ��������� ���������
	void fill_states();

	// ������� �������� ������
	void thread_fn();

	// �������������
	tools::e_init_state init();

	// �������� ���������� �� �������
	std::vector<tag_service_info> get_services_info();

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

	// �������� ������ �� ����������
	void send_counters_packet();

	// ������� ������ � ��������������
	void send_confirmation_packet(server_exchange::e_packet_type packet_type, server_exchange::e_processing_result result);

	// �������� ������ ��������� ���������
	void send_terminal_state_packet(const e_terminal_state& state);

	// ��������� ��������� ���������
	void change_terminal_state(const e_terminal_state& state, bool send_to_server = false, bool write_to_file = false);

	// ����������, ���� ���������� ����� ����������
	void on_device_not_available();

	// �������� ���������
	virtual std::shared_ptr<IState> get_state(e_state state) final;

	// ���������� ���������
	virtual void set_state(e_state state) final;

	virtual void send_issue_coins_packet_to_device(byte count) final;

	virtual void coin_issued(byte rest_of_coins) final;

	virtual void on_empty_hopper() final;

	// ������ � ��������� ��������� �� ����������
	void process_messages_from_device();

	// ������� ��������� �� ����������
	void process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message);

	template<typename _MessageType>
	_MessageType* get_device_message_pointer(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
	{
		return dynamic_cast<_MessageType*>(message.get());
	}

	// �������� ��������� �� ���������
	template<typename _StateType>
	_StateType* get_implemented_state(e_state state)
	{
		_StateType* result = dynamic_cast<_StateType*>(get_state(state).get());

		if (result->get_this_state() != state)
		{
			__debugbreak();
			throw std::exception("(_this_state != state) || (_StateType::get_this_state() != state)");
		}

		return result;
	}

	// �������� ������ ��� �������� �������
	template<typename _SourcePacketType, server_exchange::e_packet_type _Id>
	std::shared_ptr<logic_structures::tag_base_server_logic_struct> create_server_packet(_SourcePacketType packet)
	{
		logic_structures::tag_server_logic_packet <_SourcePacketType, _Id>*
			server_logic_packet = new logic_structures::tag_server_logic_packet <_SourcePacketType, _Id>(packet);

		return std::shared_ptr<logic_structures::tag_base_server_logic_struct>(server_logic_packet);
	}

	void process_messages_from_server();

	bool process_server_message(std::shared_ptr<logic_structures::tag_base_server_logic_struct> message, server_exchange::e_processing_result& processing_result);

	template<typename _MessageType, server_exchange::e_packet_type _Id>
	_MessageType get_server_message(std::shared_ptr<logic_structures::tag_base_server_logic_struct> message)
	{
		logic_structures::tag_server_logic_packet < _MessageType, _Id > * p_message =
			dynamic_cast<logic_structures::tag_server_logic_packet < _MessageType, _Id >*>(message.get());

		return p_message->packet;
	}

	// �������� ��������� ���������� ����������� � �������
	void update_device_settings_from_server();

	// ������ � ����
	void distribute_prize();

	virtual void open_valve(byte number) final;

	virtual void read_eeprom(byte cell_number) final;

	virtual void write_eeprom(byte cell_number, uint32_t value) final;

	virtual void time_and_money(int16_t time, int32_t money) final;

	virtual void close_valve(byte number) final;

	virtual void on_settings_readed() final;

	virtual void on_counters_changed() final;

	virtual void show_advertising() final;

	virtual void read_buttons_status() final;

	virtual void show_counters() final;

	virtual void issue_discount_card() final;

	virtual void send_log_to_server(server_exchange::e_log_record_type type, const std::wstring& text) final;

	virtual void send_distribute_prize_packet_to_server(server_exchange::e_distribute_element_status status, uint16_t size) final;

	virtual void send_distribute_discount_card_packet_to_server(server_exchange::e_distribute_element_status status) final;

	// ������ � ������
	virtual bool Start() final;

	// ���������
	virtual void Stop() final;

	virtual void SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t) > fn) final;

	virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring) > fn) final;

	virtual void SetOnStateChangedFn(std::function<void(e_state)> fn) final;

	virtual void SetOnCacheRefilledFn(std::function<void(uint16_t) > fn) final;

	virtual void SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>) > fn) final;

	virtual void SetOnDistributionPrizeFn(std::function<void(int16_t, byte) > fn) final;

	virtual void SetOnEmptyHopperFn(std::function<void(void) > fn) final;

	virtual void SetOnShowAdvertisingFn(std::function<void(void) > fn) final;

	virtual void SetOnTerminalStateChangedFn(std::function<void(logic::e_terminal_state) > fn) final;

	virtual void SetOnShowCountersFn(std::function<void(std::vector<tag_service_counter>) > fn) final;

public:
	CLogic();
	~CLogic();

};
}



