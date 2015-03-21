#pragma once
#include "TraceError.h"
#include "tools_structures.h"
#include "CommonSettings.h"
#include "CorrespondSettings.h"

/*!
 * \file Logic.h
 *
 * \author Victor Mulika
 * \date 21 Март 2015
 *
 * 
 */

namespace logic
{
/*
CLogic
логика работы терминала
*/
class CLogic
{
	tools::logging::CTraceError* _tr_error;

	tools::e_work_loop_status _work_loop_status;

	tools::e_init_state _init_state;

	// объект потока
	std::thread _this_thread;

	// модуль общих натроек
	logic_settings::CCommonSettings _common_settings;

	// модуль соответствий услуг, кнопок и клапанов
	logic_settings::CCorrespondSettings _correspond_settings;

	// функция текущего потока
	void thread_fn();

	// инициализация
	tools::e_init_state init();

public:
	CLogic();
	~CLogic();

	// запуск в работу
	bool Start();

	// остановка
	void Stop();
};
}



