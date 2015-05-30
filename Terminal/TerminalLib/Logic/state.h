#pragma once
#include "stdafx.h"
#include "ILogic.h"
#include "ServicesNames.h"
#include "protocol_structures.h"
#include "data_from_device.h"

/*!
 * \file state.h
 *
 * \author Victor Mulika
 * \date 22 ���� 2015
 *
 * ���������� ��������� �������� State
 */

namespace logic
{



class CLogicAbstract;

// ����������� ����� ���������
class IState
{
protected:
	// ������
	CLogicAbstract& _logic;

	// ��� ���������
	const e_state _this_state;

	// �������� ��������� �� ���������
	template<typename _StateType>
	_StateType* get_implemented_state(e_state state)
	{
		_StateType* result = dynamic_cast<_StateType*>(_logic.get_state(state).get());

		if (result->get_this_state() != state)
		{
			__debugbreak();
			throw std::exception("(_this_state != state) || (_StateType::get_this_state() != state)");
		}

		return result;
	}

	IState(CLogicAbstract& logic, const e_state& state)
		: _logic(logic)
		, _this_state(state)
	{

	}

public:

	e_state get_this_state() const
	{
		return _this_state;
	}

	// �������� ��� ��������� ���������
	virtual void activate() = 0;

	// �������� ��� ������ �� ���������
	virtual void deactivate() = 0;

	// �������� ����
	virtual void refilled_cache() = 0;

	// ������ ������ ������
	virtual void service_button_press(e_service_name service_name) = 0;

	// ������ ������ ����
	virtual void stop_button_press() = 0;

	// ����� �����
	virtual void time_out() = 0;

	// ����������� ������
	virtual void out_of_money() = 0;

	// ������������� �����������
	virtual void device_confirm() = 0;

	// ������ �� ����������
	virtual void device_error(logic_structures::e_device_error_code code) = 0;
};


class CLogicAbstract
{
protected:

	// ������� ���������
	std::shared_ptr<IState>  _state;

public:

	// ���������� ������� ���������
	virtual void set_state(e_state state) = 0;

	// �������� ��������� ���������
	virtual std::shared_ptr<IState> get_state(e_state state) = 0;

	// ������� ������
	virtual void open_valve(byte number) = 0;

	// ������� ������
	virtual void close_valve(byte number) = 0;

	// ������ ������ eeprom
	virtual void read_eeprom(byte cell_number) = 0;

	// ������ ������ eeprom
	virtual void write_eeprom(byte cell_number, uint32_t value) = 0;

	// ����������� �� ���������� �� ����� ������� � ������� ������
	// ����� � ��������, ������ � ��������
	virtual void time_and_money(int16_t time, int16_t money) = 0;

	// �������� ��� ��������� ������ ��������
	virtual void on_settings_readed() = 0;

	// �������� ��� ��������� ��������
	virtual void on_counters_changed() = 0;

	// ������ �������
	virtual void send_issue_coins_packet_to_device(byte count) = 0;

	// ������� ������, �������� ������ balance
	virtual void coin_issued(byte rest_of_coins) = 0;

	// ������ �������
	virtual void on_empty_hopper() = 0;

	// �������� ��������� �����
	virtual void show_advertising() = 0;

	// ��������� ��������� ���� ������
	virtual void read_buttons_status() = 0;

	// �������� �������� ���������
	virtual void show_counters() = 0;
};

#pragma pack(push, 4)
// ���������, ���������� �� ����������
struct tag_device_settings
{
	uint32_t	total_cache;				// ����� ����
	int32_t		current_cache;				// ������� ������ (� ��������)
	uint32_t	bill_acceptor_impulse;		// ������� ��������������		
	uint32_t	coin_acceptor_impulse;		// ������� ��������������		
	uint32_t	free_idle_time;				// ����� �������				
	uint32_t	idle_time_cost;				// ��������� ������ �������	
	uint32_t	pause_before_advertising;	// ����� ����� ������� �������
	uint32_t	state;						// ��������� (��������, �������, �������)
	uint32_t	frost_protection_value;		// ������ �� ����������
	uint32_t	discount_card_condition;	// ����� ��� ������ ���������� �����
	uint32_t	cost_pressurized_water;		// ��������� ���� ��� ���������
	uint32_t	cost_water_without_pressure;// ��������� ���� ��� ��������
	uint32_t	cost_foam;					// ��������� ����	
	uint32_t	cost_wax;					// ��������� ����	
	uint32_t	cost_against_midges;		// ��������� �� �����
	uint32_t	cost_vacuum_cleaner;		// ��������� �������	
	uint32_t	cost_air;					// ��������� ������
	uint32_t	cost_osmosis;				// ��������� �����
	uint32_t	time_pressurized_water;		// ����� ������ ���� ��� ���������
	uint32_t	time_water_without_pressure;// ����� ������ ���� ��� ��������
	uint32_t	time_foam;					// ����� ������ ����	
	uint32_t	time_wax;					// ����� ������ ����	
	uint32_t	time_against_midges;		// ����� ������ �� �����
	uint32_t	time_vacuum_cleaner;		// ����� ������ �������	
	uint32_t	time_air;					// ����� ������ ������
	uint32_t	time_osmosis;				// ����� ������ �����

	tag_device_settings()
	{
		clear();
	}

	void clear()
	{
		ZeroMemory(this, sizeof(tag_device_settings));
	}

	bool operator==(const tag_device_settings& settings)
	{
		return ((total_cache == settings.total_cache) &&
		(current_cache == settings.current_cache) &&
		(bill_acceptor_impulse == settings.bill_acceptor_impulse) &&
		(coin_acceptor_impulse == settings.coin_acceptor_impulse) &&
		(free_idle_time == settings.free_idle_time) &&
		(idle_time_cost == settings.idle_time_cost) &&
		(pause_before_advertising == settings.pause_before_advertising) &&
		(state == settings.state) &&
		(cost_pressurized_water == settings.cost_pressurized_water) &&
		(cost_water_without_pressure == settings.cost_water_without_pressure) &&
		(cost_foam == settings.cost_foam) &&
		(cost_wax == settings.cost_wax) &&
		(cost_against_midges == settings.cost_against_midges) &&
		(cost_vacuum_cleaner == settings.cost_vacuum_cleaner) &&
		(cost_air == settings.cost_air) &&
		(cost_osmosis == settings.cost_osmosis) &&
		(time_pressurized_water == settings.time_pressurized_water) &&
		(time_water_without_pressure == settings.time_water_without_pressure) &&
		(time_foam == settings.time_foam) &&
		(time_wax == settings.time_wax) &&
		(time_against_midges == settings.time_against_midges) &&
		(time_vacuum_cleaner == settings.time_vacuum_cleaner) &&
		(time_air == settings.time_air) &&
		(time_osmosis == settings.time_osmosis));
	}

};
#pragma pack(pop)

}