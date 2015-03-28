#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CFreeEdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	tag_device_settings _settings;

	Concurrency::timer<int32_t>* _timer;

	Concurrency::call<int32_t> _on_timer_call;

	// גûחûגאועסÿ עאילונמל
	void on_timer(int32_t);


public:
	CFreeEdleState(CLogicAbstract& logic);
	virtual ~CFreeEdleState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

};
}



