#include "stdafx.h"
#include "AdvertisingIdleState.h"
#include "RefillCacheState.h"
#include "SettingsWorkState.h"


bool logic::CAdvertisingIdleState::read_services_cost(tag_device_settings& settings)
{
	settings.cost_against_midges = _correspond_settings.GetServiceCost(e_service_name::against_midges);
	settings.cost_air = _correspond_settings.GetServiceCost(e_service_name::air);
	settings.cost_foam = _correspond_settings.GetServiceCost(e_service_name::foam);
	settings.cost_osmosis = _correspond_settings.GetServiceCost(e_service_name::osmosis);
	settings.cost_pressurized_water = _correspond_settings.GetServiceCost(e_service_name::pressurized_water);
	settings.cost_vacuum_cleaner = _correspond_settings.GetServiceCost(e_service_name::vacuum_cleaner);
	settings.cost_water_without_pressure = _correspond_settings.GetServiceCost(e_service_name::water_without_pressure);
	settings.cost_wax = _correspond_settings.GetServiceCost(e_service_name::wax);

	return true;

}

bool logic::CAdvertisingIdleState::read_terminal_settings(tag_device_settings& settings)
{
	settings.bill_acceptor_impulse = _common_settings.GetBillAcceptorImpulse();
	settings.coin_acceptor_impulse = _common_settings.GetCoinAcceptorImpulse();
	settings.free_idle_time = _common_settings.GetFreeIdleTime();
	settings.idle_time_cost = _common_settings.GetIdleTimeCost();
	settings.pause_before_advertising = _common_settings.GetPauseBeforeAdvertising();
	settings.state = _common_settings.GetState();

	return true;
}

void logic::CAdvertisingIdleState::on_timer(uint32_t)
{
	stop_timer();

	_logic.show_advertising();
}

void logic::CAdvertisingIdleState::stop_timer()
{
	if (nullptr != _timer)
	{
		_timer->stop();
		delete _timer;
		_timer = nullptr;
	}
}

logic::CAdvertisingIdleState::CAdvertisingIdleState(CLogicAbstract& logic,
													logic_settings::CCorrespondSettings& correspond_settings,
													logic_settings::CCommonSettings& common_settings)
	: IState(logic, e_state::advertising_idle)
	, _correspond_settings(correspond_settings)
	, _common_settings(common_settings)
	, _timer(nullptr)
	, _on_timer_call(std::bind(std::mem_fn(&CAdvertisingIdleState::on_timer), this, std::placeholders::_1))
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CAdvertisingIdleState::~CAdvertisingIdleState()
{
}

void logic::CAdvertisingIdleState::refilled_cache()
{
	stop_timer();

	CRefillCacheState* refill_cache_state = get_implemented_state<CRefillCacheState>(e_state::refill_cache);

	refill_cache_state->refilled_cache();

	_logic.set_state(e_state::refill_cache);
}

void logic::CAdvertisingIdleState::service_button_press(e_service_name service_name)
{

}

void logic::CAdvertisingIdleState::stop_button_press()
{

}

void logic::CAdvertisingIdleState::time_out()
{
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);

	tag_device_settings default_settings = sws->get_settings();
	tag_device_settings file_settings = default_settings;

	read_services_cost(file_settings);
	read_terminal_settings(file_settings);

	if (!(default_settings == file_settings))
		sws->set_settings(file_settings);
}

void logic::CAdvertisingIdleState::out_of_money()
{

}

void logic::CAdvertisingIdleState::device_confirm()
{

}

void logic::CAdvertisingIdleState::device_error(logic_structures::e_device_error_code code)
{

}

void logic::CAdvertisingIdleState::activate()
{
	stop_timer();

	uint32_t period = _common_settings.GetPauseBeforeAdvertising();

	if (0 != period)
	{
		period = period * 60 * 1000;
		_timer = new Concurrency::timer<int32_t>(period, 0, &_on_timer_call, false);
		_timer->start();
	}
	else
	{
		_logic.show_advertising();
	}
}
