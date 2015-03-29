#include "stdafx.h"
#include "FreeEdleState.h"
#include "SettingsWorkState.h"
#include "ExecutingServiceState.h"
#include "PaidIdleState.h"


void logic::CFreeEdleState::on_timer(int32_t)
{
	stop_timer();

	go_to_paid_idle_state();
}

void logic::CFreeEdleState::stop_timer()
{
	if (nullptr != _timer)
	{
		_timer->stop();
		delete _timer;
		_timer = nullptr;
	}
}

void logic::CFreeEdleState::go_to_paid_idle_state()
{
	CPaidIdleState* pis = get_implemented_state<CPaidIdleState>(e_state::paid_idle);

	_logic.set_state(e_state::paid_idle);
	pis->time_out();
}

logic::CFreeEdleState::CFreeEdleState(CLogicAbstract& logic)
	: IState(logic, e_state::free_idle)
	,_timer(nullptr)
	, _on_timer_call(std::bind(std::mem_fn(&CFreeEdleState::on_timer), this, std::placeholders::_1))
{
}

logic::CFreeEdleState::~CFreeEdleState()
{
}

void logic::CFreeEdleState::refilled_cache(uint16_t cache)
{

}

void logic::CFreeEdleState::service_button_press(e_service_name service_name)
{
	stop_timer();

	CExecutingServiceState* ess = get_implemented_state<CExecutingServiceState>(e_state::executing_service);

	_logic.set_state(e_state::executing_service);
	ess->service_button_press(service_name);
}

void logic::CFreeEdleState::stop_button_press()
{
	if (nullptr != _timer)
		return;

	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	_settings = sws->get_settings();

	if (0 != _settings.free_idle_time)
	{
		uint32_t period = _settings.free_idle_time * 60 * 1000;
		_timer = new Concurrency::timer<int32_t>(period, 0, &_on_timer_call, false);
		_timer->start();
	}
	else
	{
		go_to_paid_idle_state();
	}
}

void logic::CFreeEdleState::time_out()
{

}

void logic::CFreeEdleState::out_of_money()
{

}

void logic::CFreeEdleState::device_confirm()
{

}
