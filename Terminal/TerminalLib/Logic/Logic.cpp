#include "stdafx.h"
#include "Logic.h"


logic::CLogic::CLogic()
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

logic::CLogic::~CLogic()
{
}
