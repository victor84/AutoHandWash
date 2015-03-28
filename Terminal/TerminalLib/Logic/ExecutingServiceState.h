#pragma once
#include "state.h"
#include "CorrespondSettings.h"

namespace logic
{
class CExecutingServiceState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// ������ ������������ �����, ������ � ��������
	 logic_settings::CCorrespondSettings _correspond_settings;

	 // ������� ����������� ������
	 e_service_name _current_service;

	 // ��������� ����������
	 tag_device_settings _device_settings;

	 // ���������� ����� ������ � ��������
	 int16_t _service_time_left;

	 // ���������� ������ (� ��������)
	 int16_t _balance_of_money;

	 // ��������� ������� ������
	 int16_t _current_service_cost;

	 Concurrency::timer<int32_t>* _timer;

	 Concurrency::call<int32_t> _on_timer_call;

	 // ���������� ��������
	 void on_timer(int32_t);

	 // ���������� ������� ����� � �������
	 void calc_time_and_money();

	 // ���������� ������� ����� � ������������ � ���������� ��������
	 void calc_money_balance_by_time_left();

public:
	CExecutingServiceState(CLogicAbstract& logic);
	virtual ~CExecutingServiceState();

	virtual void refilled_cache(uint16_t cache) final;

	virtual void service_button_press(e_service_name sevice_name) final;

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

};
}



