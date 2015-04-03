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
		// ��������� ���������
		virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring)> fn) = 0;
	};


}
