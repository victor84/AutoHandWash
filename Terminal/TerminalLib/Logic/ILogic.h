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
	// изменении услуги
	virtual void SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring)> fn) = 0;

	// назначение функтора, вызываемого при
	// изменении состояния логики
	virtual void SetOnStateChangedFn(std::function<void(e_state)> fn) = 0;

	// назначение функтора, вызываемого при
	// пополнении счёта
	virtual void SetOnCacheRefilledFn(std::function<void(uint16_t)> fn) = 0;

	// назначение функтора, вызываемого для
	// передачи названий услуг и их стоимости
	virtual void SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>)> fn) = 0;

	// назначение функтора, вызываемого при
	// выдаче приза
	virtual void SetOnDistributionPrizeFn(std::function<void(int16_t, byte)> fn) = 0;

	// назначение функтора, вызываемого при
	// опустевшем хоппере
	virtual void SetOnEmptyHopperFn(std::function<void(void)> fn) = 0;

	// назначение функтора, вызываемого для
	// показа рекламы
	virtual void SetOnShowAdvertisingFn(std::function<void(void)> fn) = 0;

	// назначение функтора, вызываемого при
	// изменении состояние терминала
	virtual void SetOnTerminalStateChangedFn(std::function<void(logic::e_terminal_state)> fn) = 0;

	virtual ~ILogic()
	{

	}
};


}
