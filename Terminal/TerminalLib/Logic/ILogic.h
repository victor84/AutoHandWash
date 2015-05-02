#pragma once
#include <functional>
#include <stdint.h>
#include <string>
#include "ServicesNames.h"

namespace logic
{

// ��������� ������ ��� ������������� �������� ������������
struct ILogic
{
	// ������ � ������
	virtual bool Start() = 0;

	// ���������
	virtual void Stop() = 0;

	// ���������� ��������, ������� ���������� ���
	// ��������� ������� � �����
	virtual void SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ��������� ������
	virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ��������� ��������� ������
	virtual void SetOnStateChangedFn(std::function<void(e_state)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ���������� �����
	virtual void SetOnCacheRefilledFn(std::function<void(uint16_t)> fn) = 0;

	// ���������� ��������, ����������� ���
	// �������� �������� ����� � �� ���������
	virtual void SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ������ �����
	virtual void SetOnDistributionPrizeFn(std::function<void(int16_t, byte)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ���������� �������
	virtual void SetOnEmptyHopperFn(std::function<void(void)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ������ �������
	virtual void SetOnShowAdvertisingFn(std::function<void(void)> fn) = 0;

	// ���������� ��������, ����������� ���
	// ��������� ��������� ���������
	virtual void SetOnTerminalStateChangedFn(std::function<void(logic::e_terminal_state)> fn) = 0;

	virtual ~ILogic()
	{

	}
};


}
