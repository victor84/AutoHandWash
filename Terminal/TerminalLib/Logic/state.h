#pragma once
#include "stdafx.h"
#include "ServicesNames.h"
#include "protocol_structures.h"

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

// состояние работы логики
enum class e_state
{
	advertising_idle,	// рекламный простой
	refill_cache,		// денежный баланс пополнен
	executing_service,	// выполнение службы (услуги)
	free_idle,			// бесплатный простой
	paid_idle,			// платный простой
	settings_work,		// работа с настройками
};

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

	// пополнен счёт
	virtual void refilled_cache(uint16_t cache) = 0;

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
};
#pragma pack(pop)

}