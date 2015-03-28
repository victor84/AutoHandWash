#pragma once
#include "stdafx.h"
#include "ServicesNames.h"

/*!
 * \file state.h
 *
 * \author Victor Mulika
 * \date 22 ћарт 2015
 *
 * »нтерфейсы рализации паттерна State
 */

namespace logic
{

// состо€ние работы логики
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

// абстрактный класс состо€ни€
class IState
{
protected:
	CLogicAbstract& _logic;


	template<typename _StateType>
	_StateType* get_implemented_state(e_state state)
	{
		return dynamic_cast<_StateType*>(_logic.get_state(state).get());
	}

	IState(CLogicAbstract& logic)
		: _logic(logic)
	{

	}

public:

	// пополнен счЄт
	virtual void refilled_cache(uint16_t cache) = 0;

	// нажата кнопка услуги
	virtual void service_button_press(e_service_name sevice_name) = 0;

	// нажата кнопка —топ
	virtual void stop_button_press() = 0;

	// врем€ вышло
	virtual void time_out() = 0;

	// закончились деньги
	virtual void out_of_money() = 0;

	// подтверждение устройством
	virtual void device_confirm() = 0;

	// данные от eeprom
	virtual void eeprom_data(uint32_t value) = 0;

};


class CLogicAbstract
{
protected:

	// текущее состо€ние
	std::shared_ptr<IState>  _state;

public:
	// установить текущее состо€ние
	virtual void set_state(e_state state) = 0;

	// получить указанное состо€ние
	virtual std::shared_ptr<IState> get_state(e_state state) = 0;

	// открыть клапан
	virtual void open_valve(byte number) = 0;

};




}