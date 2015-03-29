#include "stdafx.h"
#include "RefillCacheState.h"
#include "ExecutingServiceState.h"
#include "SettingsWorkState.h"

logic::CRefillCacheState::CRefillCacheState(CLogicAbstract& logic)
	: IState(logic, e_state::refill_cache)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CRefillCacheState::~CRefillCacheState()
{
}

void logic::CRefillCacheState::refilled_cache(uint16_t cache)
{
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	_device_settings = sws->get_settings();

	_device_settings.current_cache += static_cast<int16_t>(cache * 100);

	_str_str.str(std::wstring());

	_str_str << _T("������ �������� �� ") << cache << _T(" � ���������� ")
		<< static_cast<float>(_device_settings.current_cache / 100.0);

	_tr_error->trace_message(_str_str.str());

	_device_settings.total_cache += cache;
	sws->set_settings(_device_settings);
}

void logic::CRefillCacheState::service_button_press(e_service_name service_name)
{
	CExecutingServiceState* state = get_implemented_state<CExecutingServiceState>(e_state::executing_service);

	state->service_button_press(service_name);

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

