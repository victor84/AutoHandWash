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

	// ��������� ��������� �� �������, ���������� ��� ��������� ������� � �����
	void SetOnTimeAndMoneyFn(void* pointer);

	// ��������� ��������� �� �������, ���������� ��� ��������� ������
	void SetOnServiceChangedFn(void* pointer);

	// ��������� ��������� �� �������, ���������� ��� ��������� ���������
	void SetOnStateChangedFn(void* pointer);

	// ��������� ��������� �� �������, ���������� ��� ���������� �����
	void SetOnCacheRefilledFn(void* pointer);

	// ��������� ��������� �� �������, ���������� ��� ������ ���������� �� �������
	void SetOnServiceInfoReadedFn(void* pointer);
};
