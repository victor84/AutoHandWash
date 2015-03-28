#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CAdvertisingIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

public:
	CAdvertisingIdleState(CLogicAbstract& logic);
	virtual ~CAdvertisingIdleState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name sevice_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() override;

	virtual void eeprom_data(uint32_t value) override;

};
}



