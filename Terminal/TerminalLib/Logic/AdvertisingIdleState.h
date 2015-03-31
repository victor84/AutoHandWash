#pragma once
#include "state.h"
#include "TraceError.h"
#include "CorrespondSettings.h"

namespace logic
{
class CAdvertisingIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// модуль соответствий услуг, кнопок и клапанов
	logic_settings::CCorrespondSettings& _correspond_settings;

	// прочитать и записать настройки по-умолчанию стоимости услуг
	bool read_services_cost(tag_device_settings& settings);

public:
	CAdvertisingIdleState(CLogicAbstract& logic, logic_settings::CCorrespondSettings& correspond_settings);
	virtual ~CAdvertisingIdleState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() override;
};
}



