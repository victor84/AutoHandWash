#pragma once
#include "TraceError.h"
#include "tools_structures.h"

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

public:
	CLogic();
	~CLogic();

	bool Start();

	void Stop();
};
}



