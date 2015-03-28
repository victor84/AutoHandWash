#include "stdafx.h"
#include "RefillCacheState.h"
#include "ExecutingServiceState.h"

logic::CRefillCacheState::CRefillCacheState(CLogicAbstract& logic)
	: IState(logic)
	, _cache(0)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CRefillCacheState::~CRefillCacheState()
{
}

void logic::CRefillCacheState::refilled_cache(uint16_t cache)
{
	_cache += static_cast<int16_t>(cache);

	_str_str.str(std::wstring());

	_str_str << _T("Баланс пополнен на ") << cache << _T(" и составляет ") << _cache;

	_tr_error->trace_message(_str_str.str());
}

void logic::CRefillCacheState::service_button_press(e_service_name sevice_name)
{
	CExecutingServiceState* state = get_implemented_state<CExecutingServiceState>(e_state::executing_service);

	state->service_button_press(sevice_name);

	_logic.set_state(e_state::executing_service);
}

void logic::CRefillCacheState::stop_button_press()
{

}

void logic::CRefillCacheState::time_out()
{

}

void logic::CRefillCacheState::out_of_money()
{

}

void logic::CRefillCacheState::device_confirm()
{

}

void logic::CRefillCacheState::eeprom_data(uint32_t value)
{

}
