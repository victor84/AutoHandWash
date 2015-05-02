#include "stdafx.h"
#include "LogicPimpl.h"
#include "Logic.h"
#include "ILogic.h"

typedef void(__stdcall *OnChangeTimeAndMoneyPtr)(int16_t, int16_t);
typedef void(__stdcall *OnServiceChangedPtr)(logic::e_service_name, const wchar_t*);
typedef void(__stdcall *OnStateChangedPtr)(logic::e_state);
typedef void(__stdcall *OnCacheRefilledPtr)(uint16_t);
typedef void(__stdcall *OnServiceInfoReadedPtr)(std::vector<logic::tag_service_info>);
typedef void(__stdcall *OnDistributionPrizePtr)(int16_t, byte);
typedef void(__stdcall *OnEmptyHopperPtr)(void);
typedef void(__stdcall *OnShowAdvertisingPtr)(void);
typedef void(__stdcall *OnTermianlStateChangedPtr)(logic::e_terminal_state);

class CLogicPimpl::CLogicIntern
{
	std::unique_ptr<logic::ILogic> _logic;

	tools::logging::CTraceError* _tr_error;
	tools::settings::CSettingsLoader* _settings_loader;

	OnChangeTimeAndMoneyPtr _on_change_time_and_money_ptr;
	OnServiceChangedPtr _on_service_changed_ptr;
	OnStateChangedPtr _on_state_changed_ptr;
	OnCacheRefilledPtr _on_cache_refilled_ptr;
	OnServiceInfoReadedPtr _on_service_info_readed_ptr;
	OnDistributionPrizePtr _on_distribution_prize_ptr;
	OnEmptyHopperPtr _on_empty_hopper_ptr;
	OnShowAdvertisingPtr _on_show_advertising_ptr;
	OnTermianlStateChangedPtr _on_terminal_state_changed_ptr;

public:
	CLogicIntern()
		: _on_change_time_and_money_ptr(nullptr)
		, _on_service_changed_ptr(nullptr)
		, _on_state_changed_ptr(nullptr)
		, _on_cache_refilled_ptr(nullptr)
		, _on_service_info_readed_ptr(nullptr)
		, _on_distribution_prize_ptr(nullptr)
		, _on_empty_hopper_ptr(nullptr)
		, _on_show_advertising_ptr(nullptr)
		, _on_terminal_state_changed_ptr(nullptr)
		, _logic(new logic::CLogic())
	{
		setlocale(LC_ALL, "Russian");
		_tr_error = tools::logging::CTraceError::get_instance();
		_settings_loader = tools::settings::CSettingsLoader::get_instance();

		_logic->SetOnTimeAndMoneyFn([this](int16_t time, int16_t money)
		{
			if (nullptr != _on_change_time_and_money_ptr)
				_on_change_time_and_money_ptr(time, money);
		});

		_logic->SetOnServiceChangedFn([this](logic::e_service_name service_id, std::wstring service_name)
		{
			if (nullptr != _on_service_changed_ptr)
				_on_service_changed_ptr(service_id, service_name.c_str());
		});

		_logic->SetOnStateChangedFn([this](logic::e_state state)
		{
			if (nullptr != _on_state_changed_ptr)
				_on_state_changed_ptr(state);
		});

		_logic->SetOnCacheRefilledFn([this](uint16_t cache)
		{
			if (nullptr != _on_cache_refilled_ptr)
				_on_cache_refilled_ptr(cache);
		});
		
		_logic->SetOnServiceInfoReadedFn([this](std::vector<logic::tag_service_info> collection)
		{
			if (nullptr != _on_service_info_readed_ptr)
				_on_service_info_readed_ptr(collection);
		});

		_logic->SetOnDistributionPrizeFn([this](int16_t prize_size, byte balance)
		{
			if (nullptr != _on_distribution_prize_ptr)
				_on_distribution_prize_ptr(prize_size, balance);
		}
		);

		_logic->SetOnEmptyHopperFn([this]()
		{
			if (nullptr != _on_empty_hopper_ptr)
				_on_empty_hopper_ptr();
		});

		_logic->SetOnShowAdvertisingFn([this]()
		{
			if (nullptr != _on_show_advertising_ptr)
				_on_show_advertising_ptr();
		});

		_logic->SetOnTerminalStateChangedFn([this](logic::e_terminal_state state)
		{
			if (nullptr != _on_terminal_state_changed_ptr)
				_on_terminal_state_changed_ptr(state);
		});
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

	void SetOnServiceInfoReaded(void* ptr)
	{
		_on_service_info_readed_ptr = static_cast<OnServiceInfoReadedPtr>(ptr);
	}

	void SetOnDistributionPrize(void* ptr)
	{
		_on_distribution_prize_ptr = static_cast<OnDistributionPrizePtr>(ptr);
	}

	void SetOnEmptyHopper(void* ptr)
	{
		_on_empty_hopper_ptr = static_cast<OnEmptyHopperPtr>(ptr);
	}

	void SetOnShowAdvertising(void* ptr)
	{
		_on_show_advertising_ptr = static_cast<OnShowAdvertisingPtr>(ptr);
	}

	void SetOnTerminalStateChanged(void* ptr)
	{
		_on_terminal_state_changed_ptr = static_cast<OnTermianlStateChangedPtr>(ptr);
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

void CLogicPimpl::SetOnServiceInfoReadedFn(void* pointer)
{
	_impl->SetOnServiceInfoReaded(pointer);
}

void CLogicPimpl::SetOnDistributionPrizeFn(void* pointer)
{
	_impl->SetOnDistributionPrize(pointer);
}

void CLogicPimpl::SetOnEmptyHopperFn(void* ptr)
{
	_impl->SetOnEmptyHopper(ptr);
}

void CLogicPimpl::SetOnShowAdvertisingFn(void* ptr)
{
	_impl->SetOnShowAdvertising(ptr);
}

void CLogicPimpl::SetOnTerminalStateChangedFn(void* ptr)
{
	_impl->SetOnTerminalStateChanged(ptr);
}

