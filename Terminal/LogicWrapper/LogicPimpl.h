#pragma once


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

	// ��������� ��������� �� �������, ���������� ��� ������ �����
	void SetOnDistributionPrizeFn(void* pointer);

	// ��������� ��������� �� �������, ���������� ��� ��������� �������
	void SetOnEmptyHopperFn(void* ptr);

	// ��������� ��������� �� �������, ���������� ��� ������ �������
	void SetOnShowAdvertisingFn(void* ptr);

	// ��������� ��������� �� �������, ���������� ��� ��������� ��������� ���������
	void SetOnTerminalStateChangedFn(void* ptr);
};
