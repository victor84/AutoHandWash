#pragma once
#include "state.h"
#include "CorrespondSettings.h"

namespace logic
{
class CExecutingServiceState : public IState
{
	tools::logging::CTraceError* _tr_error;

	// ������ ������������ �����, ������ � ��������
	 logic_settings::CCorrespondSettings& _correspond_settings;

	 // ������� ����������� ������
	 e_service_name _current_service;

	 // ��������� ����������
	 tag_device_settings _device_settings;

	 // ���������� ����� ������ � ��������
	 int16_t _service_time_left;

	 // ���������� ������ (� ��������)
	 int16_t _balance_of_money;

	 // ��������� ������� ������ (� ��������)
	 int16_t _current_service_cost;

	 // ������, ������������� ������ ������� ��� ����������� ������
	 Concurrency::timer<int32_t>* _timer;

	 // ���������� ��������
	 Concurrency::call<int32_t> _on_timer_call;

	 // ���������� ������
	 e_service_name _deferred_service;

	 // ���������� ���������� �����
	 uint16_t _deferred_cache;

	 // ���������� ��������
	 void on_timer(int32_t);

	 // ���������� ������� ����� � �������
	 void calc_time_and_money();

	 // ���������� ������� ����� � ������������ � ���������� ��������
	 void calc_money_balance_by_time_left();

	 // ��������� �������� ������ ������� ������ ������� ������
	 void increase_current_service_time();

	 // ��������� ���������� ������
	 void stop_service();

	 // ���������� ������
	 void stop_timer();

public:
	CExecutingServiceState(CLogicAbstract& logic, logic_settings::CCorrespondSettings& correspond_settings);
	virtual ~CExecutingServiceState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

};
}



