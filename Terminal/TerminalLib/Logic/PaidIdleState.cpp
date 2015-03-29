#include "stdafx.h"
#include "PaidIdleState.h"
#include "SettingsWorkState.h"
#include "ExecutingServiceState.h"


void logic::CPaidIdleState::on_timer(int32_t)
{
	--_time_left;
	calc_money_balance_by_time_left();

	if ((0 == _time_left) || (0 == _balance_of_money))
	{
		stop_timer();

		CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
		sws->set_settings(_device_settings);

		CExecutingServiceState* ess = get_implemented_state<CExecutingServiceState>(e_state::executing_service);
		_logic.set_state(e_state::executing_service);

		ess->out_of_money();
	}

	_logic.time_and_money(_time_left, _balance_of_money);
}

void logic::CPaidIdleState::calc_time_and_money()
{
	_balance_of_money = static_cast<int16_t>(_device_settings.current_cache);

	_current_cost = static_cast<int16_t>(_device_settings.idle_time_cost);

	_time_left = static_cast<int16_t>(_balance_of_money / _current_cost * 60);
	_balance_of_money *= 100;
	_current_cost *= 100;
}

void logic::CPaidIdleState::calc_money_balance_by_time_left()
{
	_balance_of_money = (_time_left / 60.0) * (_current_cost);
	_device_settings.current_cache = _balance_of_money / 100;
}

void logic::CPaidIdleState::stop_timer()
{
	if (nullptr != _timer)
	{
		_timer->stop();
		delete _timer;
		_timer = nullptr;
	}
}

logic::CPaidIdleState::CPaidIdleState(CLogicAbstract& logic)
	: IState(logic, e_state::paid_idle)
	, _on_timer_call(std::bind(std::mem_fn(&CPaidIdleState::on_timer), this, std::placeholders::_1))
	, _time_left(0)
	, _balance_of_money(0)
	, _current_cost(0)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CPaidIdleState::~CPaidIdleState()
{
}

void logic::CPaidIdleState::refilled_cache(uint16_t cache)
{
	
}

void logic::CPaidIdleState::service_button_press(e_service_name service_name)
{
	
}

void logic::CPaidIdleState::stop_button_press()
{
	
}

void logic::CPaidIdleState::time_out()
{
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	_device_settings = sws->get_settings();
	
	calc_time_and_money();

	if (nullptr == _timer)
	{
		_timer = new Concurrency::timer<int32_t>(1000, 0, &_on_timer_call, true);
	}

	_timer->start();
}

void logic::CPaidIdleState::out_of_money()
{
	
}

void logic::CPaidIdleState::device_confirm()
{
	
}
