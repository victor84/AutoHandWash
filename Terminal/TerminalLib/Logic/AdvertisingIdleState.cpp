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

logic::CAdvertisingIdleState::CAdvertisingIdleState(CLogicAbstract& logic, logic_settings::CCorrespondSettings& correspond_settings)
	: IState(logic, e_state::advertising_idle)
	, _correspond_settings(correspond_settings)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CAdvertisingIdleState::~CAdvertisingIdleState()
{
}

void logic::CAdvertisingIdleState::refilled_cache(uint16_t cache)
{
	CRefillCacheState* refill_cache_state = get_implemented_state<CRefillCacheState>(e_state::refill_cache);

	refill_cache_state->refilled_cache(cache);

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
	tag_device_settings cost_settings = default_settings;

	read_services_cost(cost_settings);

	if (!(default_settings == cost_settings))
		sws->set_settings(cost_settings);
}

void logic::CAdvertisingIdleState::out_of_money()
{

}

void logic::CAdvertisingIdleState::device_confirm()
{

}

