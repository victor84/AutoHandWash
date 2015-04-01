#include "stdafx.h"
#include "LogicPimpl.h"
#include "Logic.h"

class CLogicPimpl::CLogicIntern
{
	logic::CLogic _logic;

public:

	// запуск в работу
	bool Start()
	{
		return _logic.Start();
	}

	// остановка
	void Stop()
	{
		_logic.Stop();
	}

};

CLogicPimpl::CLogicPimpl()
{
}


CLogicPimpl::~CLogicPimpl()
{
}

bool CLogicPimpl::Start()
{
	return _impl->Start();
}

void CLogicPimpl::Stop()
{
	_impl->Stop();
}

