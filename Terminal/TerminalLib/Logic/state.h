#pragma once
#include "stdafx.h"
#include "ServicesNames.h"

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

// абстрактный класс состояния
class IState
{
public:

	// пополнен счёт
	virtual void refilled_cache(uint16_t cache) = 0;

	// нажата кнопка услуги
	virtual void service_button_press(e_service_name sevice_name) = 0;

	// нажата кнопка Стоп
	virtual void press_button_press() = 0;

	// время вышло
	virtual void time_out() = 0;

	// закончились деньги
	virtual void out_of_money() = 0;
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

};




}