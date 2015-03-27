#pragma once
#include "stdafx.h"
#include "ServicesNames.h"

/*!
 * \file state.h
 *
 * \author Victor Mulika
 * \date 22 ���� 2015
 *
 * ���������� ��������� �������� State
 */

namespace logic
{

// ��������� ������ ������
enum class e_state
{
	advertising_idle,	// ��������� �������
	refill_cache,		// �������� ������ ��������
	executing_service,	// ���������� ������ (������)
	free_idle,			// ���������� �������
	paid_idle,			// ������� �������
	settings_work,		// ������ � �����������
};

// ����������� ����� ���������
class IState
{
public:

	// �������� ����
	virtual void refilled_cache(uint16_t cache) = 0;

	// ������ ������ ������
	virtual void service_button_press(e_service_name sevice_name) = 0;

	// ������ ������ ����
	virtual void press_button_press() = 0;

	// ����� �����
	virtual void time_out() = 0;

	// ����������� ������
	virtual void out_of_money() = 0;
};


class CLogicAbstract
{
protected:

	// ������� ���������
	std::shared_ptr<IState>  _state;

public:
	// ���������� ������� ���������
	virtual void set_state(e_state state) = 0;

	// �������� ��������� ���������
	virtual std::shared_ptr<IState> get_state(e_state state) = 0;

};




}