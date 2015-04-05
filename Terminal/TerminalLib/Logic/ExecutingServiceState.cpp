#include "stdafx.h"
#include "ExecutingServiceState.h"
#include "SettingsWorkState.h"
#include "FreeEdleState.h"
#include "RefillCacheState.h"


void logic::CExecutingServiceState::on_timer(int32_t)
{
	--_service_time_left;
	increase_current_service_time();
	calc_money_balance_by_time_left();

	_device_settings.current_cache = _balance_of_money;
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	sws->set_settings(_device_settings);

	if ((0 >= _service_time_left) || (0 >= _balance_of_money))
	{
		out_of_money();
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
	
	_service_time_left = static_cast<int16_t>(static_cast<double>(_balance_of_money / 100.0) /
											  static_cast<double>(_current_service_cost) * 60.0);
	_balance_of_money;
	_current_service_cost *= 100;
}

void logic::CExecutingServiceState::calc_money_balance_by_time_left()
{
	_balance_of_money = static_cast<int16_t>((_service_time_left / 60.0) * (_current_service_cost));
	_device_settings.current_cache = _balance_of_money;
}

void logic::CExecutingServiceState::increase_current_service_time()
{
	switch (_current_service)
	{
		case logic::e_service_name::pressurized_water:
			++_device_settings.time_pressurized_water;
			break;
		case logic::e_service_name::water_without_pressure:
			++_device_settings.time_water_without_pressure;
			break;
		case logic::e_service_name::foam:
			++_device_settings.time_foam;
			break;
		case logic::e_service_name::wax:
			++_device_settings.time_wax;
			break;
		case logic::e_service_name::against_midges:
			++_device_settings.time_against_midges;
			break;
		case logic::e_service_name::vacuum_cleaner:
			++_device_settings.time_vacuum_cleaner;
			break;
		case logic::e_service_name::air:
			++_device_settings.time_air;
			break;
		case logic::e_service_name::osmosis:
			++_device_settings.time_osmosis;
			break;
		default:
			_tr_error->trace_error(_T("Включена неизвестная услуга"));
			break;
	}
}

void logic::CExecutingServiceState::stop_service()
{
	stop_timer();

	byte valve_number = _correspond_settings.GetValveNumber(_current_service);

	if (0 == valve_number)
		valve_number = 0xff;

	_logic.close_valve(valve_number);

	calc_money_balance_by_time_left();

	_logic.time_and_money(_service_time_left, _balance_of_money);

	_current_service = e_service_name::stop;
}

void logic::CExecutingServiceState::stop_timer()
{
	if (nullptr != _timer)
	{
		_timer->stop();
		delete _timer;
		_timer = nullptr;
	}
}

logic::CExecutingServiceState::CExecutingServiceState(CLogicAbstract& logic, logic_settings::CCorrespondSettings& correspond_settings)
	: IState(logic, e_state::executing_service)
	, _current_service(e_service_name::stop)
	, _current_service_cost(0)
	, _balance_of_money(0)
	, _service_time_left(0)
	, _timer(nullptr)
	, _deferred_cache(0)
	, _deferred_service(e_service_name::stop)
	, _correspond_settings(correspond_settings)
	, _on_timer_call(std::bind(std::mem_fn(&CExecutingServiceState::on_timer), this, std::placeholders::_1))
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CExecutingServiceState::~CExecutingServiceState()
{
	stop_service();
}

void logic::CExecutingServiceState::refilled_cache()
{
	stop_service();
	_deferred_cache = static_cast<uint16_t>(_device_settings.bill_acceptor_impulse);
}

void logic::CExecutingServiceState::service_button_press(e_service_name service_name)
{
	if (e_service_name::stop != _current_service)
	{
		stop_service();

		_deferred_service = service_name;
		return;
	}

	if (e_service_name::stop != _deferred_service)
	{
		_deferred_service = e_service_name::stop;
	}

	byte button_number = _correspond_settings.GetButtonNumber(service_name);
	byte valve_number = _correspond_settings.GetValveNumber(button_number);

	_logic.open_valve(valve_number);

	_current_service = service_name;

	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	_device_settings = sws->get_settings();
}

void logic::CExecutingServiceState::stop_button_press()
{
	stop_service();

	_logic.set_state(e_state::free_idle);
	_logic.get_state(e_state::free_idle)->stop_button_press();
}

void logic::CExecutingServiceState::time_out()
{

}

void logic::CExecutingServiceState::out_of_money()
{
	_service_time_left = 0;
	_balance_of_money = 0;

	stop_service();
}

void logic::CExecutingServiceState::device_confirm()
{
	if (e_service_name::stop != _current_service)
	{
		calc_time_and_money();
		_logic.time_and_money(_service_time_left, _balance_of_money);

		if (nullptr == _timer)
		{
			_timer = new Concurrency::timer<int32_t>(1000, 0, &_on_timer_call, true);
		}

		_timer->start();
	}
	else
	{
		if (e_service_name::stop != _deferred_service)
		{
			service_button_press(_deferred_service);
		}

		if ((_service_time_left <= 0) || (_balance_of_money <= 0))
		{
			CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
			sws->set_settings(_device_settings);
			sws->write_settings();

			CRefillCacheState* rcs = get_implemented_state<CRefillCacheState>(e_state::refill_cache);
			rcs->out_of_money();
			_logic.set_state(e_state::settings_work);
		}

		if (0 != _deferred_cache)
		{
			CRefillCacheState* rcs = get_implemented_state<CRefillCacheState>(e_state::refill_cache);
			rcs->refilled_cache();
			_deferred_cache = 0;
			_logic.set_state(e_state::refill_cache);
		}
	}
}


