#include "stdafx.h"
#include "ExecutingServiceState.h"


logic::CExecutingServiceState::CExecutingServiceState(CLogicAbstract& logic)
	: IState(logic)
{
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
}

void logic::CExecutingServiceState::stop_button_press()
{

}

void logic::CExecutingServiceState::time_out()
{

}

void logic::CExecutingServiceState::out_of_money()
{

}
