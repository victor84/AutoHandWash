// LogicWrapper.h

#pragma once

#include "LogicPimpl.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace LogicWrapper
{
	// Идентификатор услуги
	public enum e_service_id
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

	// делегат, вызываемый при изменении времени и денег
	public delegate void OnTimeAndMoneyChangedDelegate(UInt16 time, UInt16 money);

	// делегат, вызываемый при изменении услуги
	public delegate void OnServiceChanged(e_service_id service_id, String^ service_name);

	// Логика
	public ref class Logic
	{

		CLogicPimpl* _logic;
		OnTimeAndMoneyChangedDelegate^ _on_time_and_money_changed;
		OnServiceChanged^ _on_service_changed;

		GCHandle _tmc_handle;
		GCHandle _sc_handle;

		void OnServiceChangedInner(Int32 service_id, const wchar_t* service_name)
		{
			if (nullptr == _on_service_changed)
				return;

			e_service_id sid = static_cast<e_service_id>(service_id);
			String^ sn = gcnew String(service_name);
			
			_on_service_changed(sid, sn);
		}

		void TransmitDelegates()
		{
			if (false == _tmc_handle.IsAllocated)
				_tmc_handle = GCHandle::Alloc(_on_time_and_money_changed);

			if (false == _sc_handle.IsAllocated)
				_sc_handle = GCHandle::Alloc(_on_service_changed);

			_logic->SetOnTimeAndMoneyFn(Marshal::GetFunctionPointerForDelegate(_on_time_and_money_changed).ToPointer());
			_logic->SetOnServiceChangedFn(Marshal::GetFunctionPointerForDelegate(_on_service_changed).ToPointer());

		}

	public:
		Logic()
		{
			_logic = new CLogicPimpl();
		}

		~Logic()
		{
			delete _logic;
		}

		void SetDelegates(OnTimeAndMoneyChangedDelegate^ on_time_and_money_changed,
						  OnServiceChanged^ on_service_changed)
		{
			_on_time_and_money_changed = on_time_and_money_changed;
			_on_service_changed = on_service_changed;

			TransmitDelegates();
		}

		bool Start()
		{
			return _logic->Start();
		}

		void Stop()
		{
			_logic->Stop();
		}

	};
}
