// LogicWrapper.h

#pragma once

#include "LogicPimpl.h"
#include "ServicesNames.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace LogicWrapper
{
	// ������������� ������
	public enum class e_service_id
	{
		pressurized_water,		// ���� ��� ���������
		water_without_pressure,	// ���� ��� ��������
		foam,					// ����	
		wax,					// ����	
		against_midges,			// �� �����
		vacuum_cleaner,			// �������	
		air,					// ������
		osmosis,				// �����
		stop					// ����
	};

	public enum class e_state_id
	{
		advertising_idle,	// ��������� �������
		refill_cache,		// �������� ������ ��������
		executing_service,	// ���������� ������ (������)
		free_idle,			// ���������� �������
		paid_idle,			// ������� �������
		settings_work,		// ������ � �����������
	};

	public ref struct tag_service_info
	{
		String^			service_name;	// �������� ������
		UInt16			cost;			// ��������� ������
		Byte			button_number;	// ����� ������
		e_service_id	id;				// ������������� ������
	};

	// �������, ���������� ��� ��������� ������� � �����
	public delegate void OnTimeAndMoneyChangedDelegate(UInt16 time, UInt16 money);

	// �������, ���������� ��� ��������� ������
	public delegate void OnServiceChangedDelegate(e_service_id service_id, String^ service_name);

	// �������, ���������� ��� ��������� ��������� ������
	public delegate void OnStateChangedDelegate(e_state_id state_id);

	// �������, ���������� ��� ���������� �����
	public delegate void OnCacheRefilledDelegate(UInt16 cache);

	// �������, ���������� ��� ������ ���������� �� �������
	public delegate void OnServiceInfoReadedDelegate(System::Collections::Generic::IEnumerable<tag_service_info^>^ collection);

	// ������
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
