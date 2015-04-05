#pragma once
#include "stdafx.h"
#include "TraceError.h"
#include "SettingsLoader.h"

/*!
 * \file CommonSettings.h
 *
 * \author Victor Mulika
 * \date 21 Март 2015
 *
 * 
 */

namespace logic_settings
{
/*
CCommonSettings
возвращает общие настройки
*/
class CCommonSettings
{
	const wchar_t* device_block_name	= _T("device");
	const wchar_t* server_block_name	= _T("server");
	const wchar_t* terminal_block_name	= _T("terminal");

	const wchar_t* address_param_name	= _T("address");
	const wchar_t* port_param_name		= _T("port");
	const wchar_t* terminal_name_name	= _T("name");
	const wchar_t* terminal_group_name	= _T("group");

	const wchar_t* bill_acceptor_impulse_name		= _T("bill_acceptor_impulse");
	const wchar_t* coin_acceptor_impulse_name		= _T("coin_acceptor_impulse");
	const wchar_t* free_idle_time_name				= _T("free_idle_time");
	const wchar_t* idle_time_cost_name				= _T("idle_time_cost");
	const wchar_t* pause_before_advertising_name	= _T("pause_before_advertising");
	const wchar_t* state_name						= _T("state");

	tools::logging::CTraceError* _tr_error;
	tools::settings::CSettingsLoader* _settings_module;

	std::string _device_port;

	std::string _server_port;
	std::string _server_address;

	std::wstring _terminal_name;
	std::wstring _terminal_group;

	uint32_t _bill_acceptor_impulse;
	uint32_t _coin_acceptor_impulse;
	uint32_t _free_idle_time;
	uint32_t _idle_time_cost;
	uint32_t _pause_before_advertising;
	uint32_t _state;

	void fill_settings_values();

public:
	CCommonSettings();
	virtual ~CCommonSettings();

	bool ReadSettings();

	std::string GetDevicePort() const;

	std::string GetServerAddress() const;

	std::string GetServerPort() const;

	std::wstring GetTerminalName() const;

	std::wstring GetTerminalGroup() const;

	uint32_t GetBillAcceptorImpulse() const;

	uint32_t GetCoinAcceptorImpulse() const;

	uint32_t GetFreeIdleTime() const;

	uint32_t GetIdleTimeCost() const;

	uint32_t GetPauseBeforeAdvertising() const;

	uint32_t GetState() const;
};

}