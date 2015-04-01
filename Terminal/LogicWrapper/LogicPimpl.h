#pragma once
#include "Stdafx.h"


class CLogicPimpl
{
	class CLogicIntern;

	std::unique_ptr<CLogicIntern> _impl;

public:
	CLogicPimpl();
	virtual ~CLogicPimpl();

	// запуск в работу
	bool Start();

	// остановка
	void Stop();

};
