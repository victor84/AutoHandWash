#pragma once
#include <functional>
#include <stdint.h>
#include <string>
#include "ServicesNames.h"

namespace logic
{



	// интерфейс логики для использования внешними компонентами
	struct ILogic
	{
		// запуск в работу
		virtual bool Start() = 0;

		// остановка
		virtual void Stop() = 0;

		// назначение функтора, который вызывается при
		// изменении времени и денег
		virtual void SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t)> fn) = 0;

		// назначение функтора, вызываемого при
		// изменении состояния
		virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring)> fn) = 0;
	};


}
