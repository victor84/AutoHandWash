#pragma once
#include "state.h"
#include "TraceError.h"
#include "CorrespondSettings.h"
#include "CommonSettings.h"

namespace logic
{
class CAdvertisingIdleState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// модуль соответствий услуг, кнопок и клапанов
	logic_settings::CCorrespondSettings& _correspond_settings;

	// модуль общих настроек терминала
	logic_settings::CCommonSettings& _common_settings;

	// таймер перед показом рекламы
	Concurrency::timer<int32_t>* _timer;

	// вызывается при срабатывании таймера
	Concurrency::call<int32_t> _on_timer_call;

	// прочитать и записать настройки по-умолчанию стоимости услуг
	bool read_services_cost(tag_device_settings& settings);

	// чтение настроек терминала
	bool read_terminal_settings(tag_device_settings& settings);

	// таймер сработал
	void on_timer(uint32_t);

	// остановка таймера
	void stop_timer();

public:
	CAdvertisingIdleState(CLogicAbstract& logic, 
						  logic_settings::CCorrespondSettings& correspond_settings,
						  logic_settings::CCommonSettings& common_settings);
	virtual ~CAdvertisingIdleState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

	virtual void device_error(logic_structures::e_device_error_code code) final;

	virtual void activate() override;

};
}



