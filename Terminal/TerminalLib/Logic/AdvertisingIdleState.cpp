#include "stdafx.h"
#include "AdvertisingIdleState.h"
#include "RefillCacheState.h"


logic::CAdvertisingIdleState::CAdvertisingIdleState(CLogicAbstract& logic)
	: IState(logic, e_state::advertising_idle)
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

}

void logic::CAdvertisingIdleState::out_of_money()
{

}

void logic::CAdvertisingIdleState::device_confirm()
{

}

