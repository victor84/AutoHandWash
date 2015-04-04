// LogicWrapper.h

#pragma once

#include "LogicPimpl.h"
#include "ServicesNames.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace LogicWrapper
{
	// Идентификатор услуги
	public enum class e_service_id
	{
		pressurized_water,		// Вода под давлением
		water_without_pressure,	// Вода без давления
		foam,					// Пена	
		wax,					// Воск	
		against_midges,			// От мошек
		vacuum_cleaner,			// Пылесос	
		air,					// Воздух
		osmosis,				// Осмос
		stop					// Стоп
	};

	public enum class e_state_id
	{
		advertising_idle,	// рекламный простой
		refill_cache,		// денежный баланс пополнен
		executing_service,	// выполнение службы (услуги)
		free_idle,			// бесплатный простой
		paid_idle,			// платный простой
		settings_work,		// работа с настройками
	};

	public ref struct tag_service_info
	{
		String^			service_name;	// название услуги
		UInt16			cost;			// стоимость услуги
		Byte			button_number;	// номер кнопки
		e_service_id	id;				// идентификатор услуги
	};

	// делегат, вызываемый при изменении времени и денег
	public delegate void OnTimeAndMoneyChangedDelegate(UInt16 time, UInt16 money);

	// делегат, вызываемый при изменении услуги
	public delegate void OnServiceChangedDelegate(e_service_id service_id, String^ service_name);

	// делегат, вызываемый при изменении состояния логики
	public delegate void OnStateChangedDelegate(e_state_id state_id);

	// делегат, вызываемый при пополнении счёта
	public delegate void OnCacheRefilledDelegate(UInt16 cache);

	// делегат, вызываемый при чтении информации об услугах
	public delegate void OnServiceInfoReadedDelegate(System::Collections::Generic::IEnumerable<tag_service_info^>^ collection);

	// Логика
	public ref class Logic
	{
		delegate void OnServiceChangedDelegateInner(logic::e_service_name service_id, const wchar_t* service_name);

		delegate void OnStateChangedDelegateInner(logic::e_state state);

		delegate void OnServiceInfoReadedDelegateInner(std::vector<logic::tag_service_info> collection);

		CLogicPimpl* _logic;
		
		OnTimeAndMoneyChangedDelegate^ _on_time_and_money_changed;
		
		OnServiceChangedDelegate^ _on_service_changed;
		OnServiceChangedDelegateInner^ _on_service_changed_inner;

		OnStateChangedDelegate^ _on_state_changed;
		OnStateChangedDelegateInner^ _on_state_changed_inner;

		OnCacheRefilledDelegate^ _on_cache_refilled;

		OnServiceInfoReadedDelegate^ _on_service_info_readed;
		OnServiceInfoReadedDelegateInner^ _on_service_info_readed_inner;

		GCHandle _tmc_handle;
		GCHandle _sc_handle;
		GCHandle _stc_handle;
		GCHandle _cr_handle;
		GCHandle _sir_handle;

		void OnServiceChangedInner(logic::e_service_name service_id, const wchar_t* service_name);

		void OnStateChangedInner(logic::e_state state);

		void OnServiceInfoReadedInner(std::vector<logic::tag_service_info> collection);


	public:
		Logic();

		~Logic();

		void SetDelegate(OnTimeAndMoneyChangedDelegate^ on_time_and_money_changed);

		void SetDelegate(OnServiceChangedDelegate^ on_service_changed);

		void SetDelegate(OnStateChangedDelegate^ on_state_changed);

		void SetDelegate(OnCacheRefilledDelegate^ on_cache_refilled);

		void SetDelegate(OnServiceInfoReadedDelegate^ on_service_info_readed);

		bool Start();

		void Stop();

	};
}
