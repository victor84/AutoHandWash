#pragma once
#include "Stdafx.h"


class CLogicPimpl
{
	class CLogicIntern;

	std::unique_ptr<CLogicIntern> _impl;

public:
	CLogicPimpl();
	virtual ~CLogicPimpl();

	// ������ � ������
	bool Start();

	// ���������
	void Stop();

};
