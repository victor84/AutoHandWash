#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"

/*!
 * \file Logic.h
 *
 * \author Victor Mulika
 * \date 21 ���� 2015
 *
 * 
 */

namespace logic
{
/*
CLogic
������ ������ ���������
*/
class CLogic
{
	tools::logging::CTraceError* _tr_error;

	tools::e_work_loop_status _work_loop_status;

	tools::e_init_state _init_state;

	// ������ ������
	std::thread _this_thread;

	// ������ ����� �������
	logic_settings::CCommonSettings _common_settings;

	// ������ ������������ �����, ������ � ��������
	logic_settings::CCorrespondSettings _correspond_settings;

	// ������� �������� ������
	void thread_fn();

	// �������������
	tools::e_init_state init();

public:
	CLogic();
	~CLogic();

	// ������ � ������
	bool Start();

	// ���������
	void Stop();
};
}



