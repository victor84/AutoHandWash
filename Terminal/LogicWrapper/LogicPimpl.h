#pragma once


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

	// установка указателя на функцию, вызываемую при изменении времени и денег
	void SetOnTimeAndMoneyFn(void* pointer);

	// установка указателя на функцию, вызываемую при изменении услуги
	void SetOnServiceChangedFn(void* pointer);

	// установка указателя на функцию, вызываемую при изменении состояния
	void SetOnStateChangedFn(void* pointer);

	// установка указателя на функцию, вызываемую при пополнении счёта
	void SetOnCacheRefilledFn(void* pointer);

	// установка указателя на функцию, вызываемую при чтении информации об услугах
	void SetOnServiceInfoReadedFn(void* pointer);

	// установка указателя на функцию, вызываемую при выдаче приза
	void SetOnDistributionPrizeFn(void* pointer);

	// установка указателя на функцию, вызываемую при опустении хоппера
	void SetOnEmptyHopperFn(void* ptr);

	// установка указателя на функцию, вызываемую для показа рекламы
	void SetOnShowAdvertisingFn(void* ptr);

	// установка указателя на функцию, вызываемую при изменении состояния терминала
	void SetOnTerminalStateChangedFn(void* ptr);
};
