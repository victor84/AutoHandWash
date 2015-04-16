#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CRefillCacheState : public IState
{
	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

	// вызывается при пополнении счёта
	std::function<void(uint16_t)> _on_cache_refilled;

public:
	CRefillCacheState(CLogicAbstract& logic);
	virtual ~CRefillCacheState();

	virtual void refilled_cache() override;

	virtual void service_button_press(e_service_name service_name) override;

	virtual void stop_button_press() override;

	virtual void time_out() override;

	virtual void out_of_money() override;

	virtual void device_confirm() override;

	void set_on_cache_refilled_fn(std::function<void(uint16_t) > fn);

	virtual void device_error(logic_structures::e_device_error_code code) final;

};
}