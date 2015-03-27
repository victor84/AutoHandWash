#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CAdvertisingIdleState : public IState
{
	CLogicAbstract& _logic;

	tools::logging::CTraceError* _tr_error;

public:
	CAdvertisingIdleState(CLogicAbstract& logic);
	virtual ~CAdvertisingIdleState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name sevice_name) final;

	virtual void press_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

};
}



