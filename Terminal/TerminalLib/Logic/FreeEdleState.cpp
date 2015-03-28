#include "stdafx.h"
#include "FreeEdleState.h"
#include "SettingsWorkState.h"


void logic::CFreeEdleState::on_timer(int32_t)
{
	_timer->stop();
	delete _timer;
	_timer = nullptr;

}

logic::CFreeEdleState::CFreeEdleState(CLogicAbstract& logic)
	: IState(logic)
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

}

void logic::CFreeEdleState::stop_button_press()
{
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	_settings = sws->get_settings();

	if (0 != _settings.free_idle_time)
	{
		if (nullptr == _timer)
		{
			uint32_t period = _settings.free_idle_time * 60 * 1000;
			_timer = new Concurrency::timer<int32_t>(period, 0, &_on_timer_call, false);
			_timer->start();
		}
		else
		{

		}
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
