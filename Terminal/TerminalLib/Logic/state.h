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
 * \date 22 Март 2015
 *
 * Интерфейсы рализации паттерна State
 */

namespace logic
{



class CLogicAbstract;

// абстрактный класс состояния
class IState
{
protected:
	// логика
	CLogicAbstract& _logic;

	// это состояние
	const e_state _this_state;

	// получить указатель на наследник
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

	// вызывать при активации состояния
	virtual void activate() = 0;

	// вызывать при выходе из состояния
	virtual void deactivate() = 0;

	// пополнен счёт
	virtual void refilled_cache() = 0;

	// нажата кнопка услуги
	virtual void service_button_press(e_service_name service_name) = 0;

	// нажата кнопка Стоп
	virtual void stop_button_press() = 0;

	// время вышло
	virtual void time_out() = 0;

	// закончились деньги
	virtual void out_of_money() = 0;

	// подтверждение устройством
	virtual void device_confirm() = 0;

	// ошибка от устройства
	virtual void device_error(logic_structures::e_device_error_code code) = 0;
};


class CLogicAbstract
{
protected:

	// текущее состояние
	std::shared_ptr<IState>  _state;

public:

	// установить текущее состояние
	virtual void set_state(e_state state) = 0;

	// получить указанное состояние
	virtual std::shared_ptr<IState> get_state(e_state state) = 0;

	// открыть клапан
	virtual void open_valve(byte number) = 0;

	// закрыть клапан
	virtual void close_valve(byte number) = 0;

	// читать ячейку eeprom
	virtual void read_eeprom(byte cell_number) = 0;

	// писать ячейку eeprom
	virtual void write_eeprom(byte cell_number, uint32_t value) = 0;

	// уведомление об оставшихся на счёте деньгах и времени услуги
	// время в секундах, деньги в копейках
	virtual void time_and_money(int16_t time, int16_t money) = 0;

	// вызывать при окончании чтения настроек
	virtual void on_settings_readed() = 0;

	// вызывать при изменении настроек
	virtual void on_counters_changed() = 0;

	// выдать монетки
	virtual void send_issue_coins_packet_to_device(byte count) = 0;

	// монетка выдана, осталось выдать balance
	virtual void coin_issued(byte rest_of_coins) = 0;

	// хоппер опустел
	virtual void on_empty_hopper() = 0;

	// показать рекламный ролик
	virtual void show_advertising() = 0;

	// запросить состояние всех кнопок
	virtual void read_buttons_status() = 0;

	// показать значения счётчиков
	virtual void show_counters() = 0;
};

#pragma pack(push, 4)
// настройки, хранящиеся на устройстве
struct tag_device_settings
{
	uint32_t	total_cache;				// Общий вход
	int32_t		current_cache;				// Текущий баланс (в копейках)
	uint32_t	bill_acceptor_impulse;		// Импульс купюроприёмника		
	uint32_t	coin_acceptor_impulse;		// Импульс монетоприёмника		
	uint32_t	free_idle_time;				// Время простоя				
	uint32_t	idle_time_cost;				// Стоимость минуты простоя	
	uint32_t	pause_before_advertising;	// Пауза перед показом рекламы
	uint32_t	state;						// Состояние (работает, простой, сломана)
	uint32_t	frost_protection_value;		// Защита от замерзания
	uint32_t	discount_card_condition;	// Сумма для выдачи дисконтной карты
	uint32_t	cost_pressurized_water;		// Стоимость вода под давлением
	uint32_t	cost_water_without_pressure;// Стоимость вода без давления
	uint32_t	cost_foam;					// Стоимость пена	
	uint32_t	cost_wax;					// Стоимость воск	
	uint32_t	cost_against_midges;		// Стоимость от мошек
	uint32_t	cost_vacuum_cleaner;		// Стоимость пылесос	
	uint32_t	cost_air;					// Стоимость воздух
	uint32_t	cost_osmosis;				// Стоимость осмос
	uint32_t	time_pressurized_water;		// Время работы вода под давлением
	uint32_t	time_water_without_pressure;// Время работы вода без давления
	uint32_t	time_foam;					// Время работы пена	
	uint32_t	time_wax;					// Время работы воск	
	uint32_t	time_against_midges;		// Время работы от мошек
	uint32_t	time_vacuum_cleaner;		// Время работы пылесос	
	uint32_t	time_air;					// Время работы воздух
	uint32_t	time_osmosis;				// Время работы осмос

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