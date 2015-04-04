// ֳכאגםûי DLL-פאיכ.

#include "stdafx.h"

#include "LogicWrapper.h"


void LogicWrapper::Logic::OnServiceChangedInner(logic::e_service_name service_id, const wchar_t* service_name)
{
	if (nullptr == _on_service_changed)
		return;

	e_service_id sid = static_cast<e_service_id>(service_id);
	String^ sn = gcnew String(service_name);

	_on_service_changed(sid, sn);
}

void LogicWrapper::Logic::OnStateChangedInner(logic::e_state state)
{
	if (nullptr == _on_state_changed)
		return;

	e_state_id state_id = static_cast<e_state_id>(state);

	_on_state_changed(state_id);
}

void LogicWrapper::Logic::OnServiceInfoReadedInner(std::vector<logic::tag_service_info> collection)
{
	if (nullptr == _on_service_info_readed)
		return;

	System::Collections::Generic::List<tag_service_info^>^ result = gcnew System::Collections::Generic::List<tag_service_info^>();

	for (logic::tag_service_info elem : collection)
	{
		tag_service_info^ man_elem = gcnew tag_service_info();

		man_elem->button_number = elem.button_number;
		man_elem->cost = elem.cost;
		man_elem->service_name = gcnew String(elem.service_name.c_str());

		result->Add(man_elem);
	}

	_on_service_info_readed(result);
}

void LogicWrapper::Logic::TransmitDelegates()
{
	if (false == _tmc_handle.IsAllocated)
		_tmc_handle = GCHandle::Alloc(_on_time_and_money_changed);

	if (false == _sc_handle.IsAllocated)
		_sc_handle = GCHandle::Alloc(_on_service_changed_inner);

	if (false == _stc_handle.IsAllocated)
		_stc_handle = GCHandle::Alloc(_on_state_changed_inner);

	if (false == _cr_handle.IsAllocated)
		_cr_handle = GCHandle::Alloc(_on_cache_refilled);

	if (false == _sir_handle.IsAllocated)
		_sir_handle = GCHandle::Alloc(_on_service_info_readed_inner);

	_logic->SetOnTimeAndMoneyFn(Marshal::GetFunctionPointerForDelegate(_on_time_and_money_changed).ToPointer());
	_logic->SetOnServiceChangedFn(Marshal::GetFunctionPointerForDelegate(_on_service_changed_inner).ToPointer());
	_logic->SetOnStateChangedFn(Marshal::GetFunctionPointerForDelegate(_on_state_changed_inner).ToPointer());
	_logic->SetOnCacheRefilledFn(Marshal::GetFunctionPointerForDelegate(_on_cache_refilled).ToPointer());
	_logic->SetOnServiceInfoReadedFn(Marshal::GetFunctionPointerForDelegate(_on_service_info_readed_inner).ToPointer());
}

LogicWrapper::Logic::Logic()
	: _on_service_changed(nullptr)
	, _on_time_and_money_changed(nullptr)
	, _on_state_changed(nullptr)
	, _on_cache_refilled(nullptr)
	, _on_service_info_readed(nullptr)
{
	_logic = new CLogicPimpl();
	_on_service_changed_inner = gcnew OnServiceChangedDelegateInner(this, &Logic::OnServiceChangedInner);
	_on_state_changed_inner = gcnew OnStateChangedDelegateInner(this, &Logic::OnStateChangedInner);
	_on_service_info_readed_inner = gcnew OnServiceInfoReadedDelegateInner(this, &Logic::OnServiceInfoReadedInner);
}

LogicWrapper::Logic::~Logic()
{
	delete _logic;
}

void LogicWrapper::Logic::SetDelegates(OnTimeAndMoneyChangedDelegate^ on_time_and_money_changed, 
									   OnServiceChangedDelegate^ on_service_changed,
									   OnStateChangedDelegate^ on_state_changed,
									   OnCacheRefilledDelegate^ on_cache_refilled,
									   OnServiceInfoReadedDelegate^ on_service_info_readed)
{
	_on_time_and_money_changed = on_time_and_money_changed;
	_on_service_changed = on_service_changed;
	_on_state_changed = on_state_changed;
	_on_cache_refilled = on_cache_refilled;
	_on_service_info_readed = on_service_info_readed;

	TransmitDelegates();
}

bool LogicWrapper::Logic::Start()
{
	return _logic->Start();
}

void LogicWrapper::Logic::Stop()
{
	_logic->Stop();
}
