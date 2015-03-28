#include "stdafx.h"
#include "ExecutingServiceState.h"
#include "SettingsWorkState.h"


void logic::CExecutingServiceState::on_timer(int32_t)
{
	--_service_time_left;
	calc_money_balance_by_time_left();

	if ((0 == _service_time_left) || (0 == _balance_of_money))
	{
		_timer->pause();

		CSettingsWorkState* sws = dynamic_cast<CSettingsWorkState*>(_logic.get_state(e_state::settings_work).get());
		sws->set_settings(_device_settings);
		sws->write_settings();

		_logic.set_state(e_state::settings_work);
	}

	_logic.time_and_money(_service_time_left, _balance_of_money);
}

void logic::CExecutingServiceState::calc_time_and_money()
{
	_balance_of_money = static_cast<int16_t>(_device_settings.current_cache);

	switch (_current_service)
	{
		case logic::e_service_name::pressurized_water:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_pressurized_water);
			break;
		case logic::e_service_name::water_without_pressure:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_water_without_pressure);
			break;
		case logic::e_service_name::foam:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_foam);
			break;
		case logic::e_service_name::wax:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_wax);
			break;
		case logic::e_service_name::against_midges:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_against_midges);
			break;
		case logic::e_service_name::vacuum_cleaner:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_vacuum_cleaner);
			break;
		case logic::e_service_name::air:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_air);
			break;
		case logic::e_service_name::osmosis:
			_current_service_cost = static_cast<int16_t>(_device_settings.cost_osmosis);
			break;
		default:
			_tr_error->trace_error(_T("Включена неизвестная услуга!"));
			break;
	}
	
	_service_time_left = static_cast<int16_t>(_balance_of_money / _current_service_cost * 60);
	_balance_of_money *= 100;
}

void logic::CExecutingServiceState::calc_money_balance_by_time_left()
{
	_balance_of_money = _service_time_left / 60 * _current_service_cost;
	_device_settings.current_cache = _balance_of_money;
}

logic::CExecutingServiceState::CExecutingServiceState(CLogicAbstract& logic)
	: IState(logic)
	, _current_service(e_service_name::stop)
	, _current_service_cost(0)
	, _timer(nullptr)
	, _on_timer_call(std::bind(std::mem_fn(&CExecutingServiceState::on_timer), this, std::placeholders::_1))
{
	_tr_error = tools::logging::CTraceError::get_instance();
	_correspond_settings.ReadSettings();
}

logic::CExecutingServiceState::~CExecutingServiceState()
{
}

void logic::CExecutingServiceState::refilled_cache(uint16_t cache)
{

}

void logic::CExecutingServiceState::service_button_press(e_service_name sevice_name)
{
	byte button_number = _correspond_settings.GetButtonNumber(sevice_name);
	byte valve_number = _correspond_settings.GetValveNumber(button_number);

	_logic.open_valve(valve_number);

	_current_service = sevice_name;

	CSettingsWorkState* sws = dynamic_cast<CSettingsWorkState*>(_logic.get_state(e_state::settings_work).get());
	_device_settings = sws->get_settings();
}

void logic::CExecutingServiceState::stop_button_press()
{
	if (nullptr != _timer)
	{
		_timer->pause();
	}
}

void logic::CExecutingServiceState::time_out()
{

}

void logic::CExecutingServiceState::out_of_money()
{

}

void logic::CExecutingServiceState::device_confirm()
{
	if (e_service_name::stop != _current_service)
	{
		calc_time_and_money();
		_logic.time_and_money(_service_time_left, _balance_of_money);

		if (nullptr == _timer)
		{
			_timer = new Concurrency::timer<int32_t>(1000, 
													 0, 
													 &_on_timer_call, 
													 true);
		}

		_timer->start();
	}

}


