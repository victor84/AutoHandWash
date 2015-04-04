#include "stdafx.h"
#include "LogicPimpl.h"
#include "Logic.h"
#include "ILogic.h"

typedef void(__stdcall *OnChangeTimeAndMoneyPtr)(int16_t, int16_t);
typedef void(__stdcall *OnServiceChangedPtr)(logic::e_service_name, const wchar_t*);
typedef void(__stdcall *OnStateChangedPtr)(logic::e_state);
typedef void(__stdcall *OnCacheRefilledPtr)(uint16_t);

class CLogicPimpl::CLogicIntern
{
	std::unique_ptr<logic::ILogic> _logic;

	tools::logging::CTraceError* _tr_error;
	tools::settings::CSettingsLoader* _settings_loader;

	OnChangeTimeAndMoneyPtr _on_change_time_and_money_ptr;
	OnServiceChangedPtr _on_service_changed_ptr;
	OnStateChangedPtr _on_state_changed_ptr;
	OnCacheRefilledPtr _on_cache_refilled_ptr;

	void OnChangeTimeAndMoney(int16_t time, int16_t money)
	{
		if (nullptr != _on_change_time_and_money_ptr)
			_on_change_time_and_money_ptr(time, money);
	}

	void OnServiceChanged(logic::e_service_name service_id, std::wstring service_name)
	{
		if (nullptr != _on_service_changed_ptr)
			_on_service_changed_ptr(service_id, service_name.c_str());
	}

	void OnStateChanged(logic::e_state state)
	{
		if (nullptr != _on_state_changed_ptr)
			_on_state_changed_ptr(state);
	}

	void OnCacheRefilled(uint16_t cache)
	{
		if (nullptr != _on_cache_refilled_ptr)
			_on_cache_refilled_ptr(cache);
	}

public:
	CLogicIntern()
		: _on_change_time_and_money_ptr(nullptr)
		, _on_service_changed_ptr(nullptr)
		, _on_state_changed_ptr(nullptr)
		, _on_cache_refilled_ptr(nullptr)
		, _logic(new logic::CLogic())
	{
		setlocale(LC_ALL, "Russian");
		_tr_error = tools::logging::CTraceError::get_instance();
		_settings_loader = tools::settings::CSettingsLoader::get_instance();

		_logic->SetOnTimeAndMoneyFn(std::bind(std::mem_fn(&CLogicIntern::OnChangeTimeAndMoney),
			this,
			std::placeholders::_1,
			std::placeholders::_2));

		_logic->SetOnServiceChangedFn(std::bind(std::mem_fn(&CLogicIntern::OnServiceChanged),
			this,
			std::placeholders::_1,
			std::placeholders::_2));

		_logic->SetOnStateChangedFn(std::bind(std::mem_fn(&CLogicIntern::OnStateChanged),
			this,
			std::placeholders::_1));

		_logic->SetOnCacheRefilledFn(std::bind(std::mem_fn(&CLogicIntern::OnCacheRefilled),
			this,
			std::placeholders::_1));
		
	}

	~CLogicIntern()
	{
		delete _settings_loader;
		delete _tr_error;
	}

	// запуск в работу
	bool Start()
	{
		return _logic->Start();
	}

	// остановка
	void Stop()
	{
		_logic->Stop();
	}

	void SetOnChangeTimeAndMoney(void* ptr)
	{
		_on_change_time_and_money_ptr = static_cast<OnChangeTimeAndMoneyPtr>(ptr);
	}

	void SetOnServiceChanged(void* ptr)
	{
		_on_service_changed_ptr = static_cast<OnServiceChangedPtr>(ptr);
	}

	void SetOnStateChanged(void* ptr)
	{
		_on_state_changed_ptr = static_cast<OnStateChangedPtr>(ptr);
	}

	void SetOnCacheRefilled(void* ptr)
	{
		_on_cache_refilled_ptr = static_cast<OnCacheRefilledPtr>(ptr);
	}

};

CLogicPimpl::CLogicPimpl()
	: _impl(new CLogicPimpl::CLogicIntern())
{
}

CLogicPimpl::~CLogicPimpl()
{
}

bool CLogicPimpl::Start()
{
	return _impl->Start();
}

void CLogicPimpl::Stop()
{
	_impl->Stop();
}

void CLogicPimpl::SetOnTimeAndMoneyFn(void* pointer)
{
	_impl->SetOnChangeTimeAndMoney(pointer);
}

void CLogicPimpl::SetOnServiceChangedFn(void* pointer)
{
	_impl->SetOnServiceChanged(pointer);
}

void CLogicPimpl::SetOnStateChangedFn(void* pointer)
{
	_impl->SetOnStateChanged(pointer);
}

void CLogicPimpl::SetOnCacheRefilledFn(void* pointer)
{
	_impl->SetOnCacheRefilled(pointer);
}

