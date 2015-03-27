#include "stdafx.h"
#include "AdvertisingIdleState.h"
#include "RefillCacheState.h"


logic::CAdvertisingIdleState::CAdvertisingIdleState(CLogicAbstract& logic)
	: _logic(logic)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CAdvertisingIdleState::~CAdvertisingIdleState()
{
}

void logic::CAdvertisingIdleState::refilled_cache(uint16_t cache)
{
	_tr_error->trace_message(_T("Баланс пополнен"));

	std::shared_ptr<IState> state = _logic.get_state(e_state::refill_cache);

	CRefillCacheState* refill_cache_state = dynamic_cast<CRefillCacheState*>(state.get());

	refill_cache_state->refilled_cache(cache);

	_logic.set_state(e_state::refill_cache);

}

void logic::CAdvertisingIdleState::service_button_press(e_service_name sevice_name)
{

}

void logic::CAdvertisingIdleState::press_button_press()
{

}

void logic::CAdvertisingIdleState::time_out()
{

}

void logic::CAdvertisingIdleState::out_of_money()
{

}
