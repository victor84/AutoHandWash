#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CPaidIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// настройки устройства
	tag_device_settings _device_settings;

	// таймер, срабатывающий каждую секунду для уведомления логики
	Concurrency::timer<int32_t>* _timer;

	// вызывается таймером
	Concurrency::call<int32_t> _on_timer_call;

	// оставшееся время в секундах
	int16_t _time_left;

	// оставшиеся деньги (в копейках)
	int16_t _balance_of_money;

	// стоимость простоя (в копейках)
	int16_t _current_cost;

	// вызывается таймером
	void on_timer(int32_t);

	// вычисление остатка денег и времени
	void calc_time_and_money();

	// вычисление остатка денег в соответствии с оставшимся временем
	void calc_money_balance_by_time_left();

	// остановить таймер
	void stop_timer();

public:
	CPaidIdleState(CLogicAbstract& logic);
	virtual ~CPaidIdleState();

	virtual void refilled_cache() override;

	virtual void service_button_press(e_service_name service_name) override;

	virtual void stop_button_press() override;

	virtual void time_out() override;

	virtual void out_of_money() override;

	virtual void device_confirm() override;

};
}



