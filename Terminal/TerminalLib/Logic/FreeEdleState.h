#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CFreeEdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	tag_device_settings _settings;

	Concurrency::timer<int32_t>* _timer;

	Concurrency::call<int32_t> _on_timer_call;

	// вызывается таймером
	void on_timer(int32_t);

	// остановить таймер
	void stop_timer();

	// перейти в состояние платного простоя
	void go_to_paid_idle_state();

public:
	CFreeEdleState(CLogicAbstract& logic);
	virtual ~CFreeEdleState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

};
}



