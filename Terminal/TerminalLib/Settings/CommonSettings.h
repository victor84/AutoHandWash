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

	const wchar_t* port_name_param_name	= _T("port_name");
	const wchar_t* port_param_name		= _T("port");
	const wchar_t* speed_param_name		= _T("speed");

	const wchar_t* address_param_name	= _T("address");
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

	std::wstring _device_port_name;
	uint32_t _device_speed;

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

	std::wstring GetDevicePortName() const;

	uint32_t GetDeviceSpeed() const;

	std::string GetServerAddress() const;

	std::string GetServerPort() const;

	std::wstring GetTerminalName() const;

	std::wstring GetTerminalGroup() const;

	uint32_t GetBillAcceptorImpulse() const;
	bool SetBillAcceptorImpulse(const uint32_t& val);

	uint32_t GetCoinAcceptorImpulse() const;
	bool SetCoinAcceptorImpulse(const uint32_t& val);

	uint32_t GetFreeIdleTime() const;
	bool SetFreeIdleTime(const uint32_t& val);

	uint32_t GetIdleTimeCost() const;
	bool SetIdleTimeCost(const uint32_t& val);

	uint32_t GetPauseBeforeAdvertising() const;
	bool SetPauseBeforeAdvertising(const uint32_t& val);

	uint32_t GetState() const;
	bool SetState(const uint32_t& val);
};

}