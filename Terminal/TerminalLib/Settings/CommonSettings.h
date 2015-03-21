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
	const wchar_t* device_block_name = _T("device");
	const wchar_t* server_block_name = _T("server");

	const wchar_t* address_param_name = _T("address");
	const wchar_t* port_param_name = _T("port");

	tools::logging::CTraceError* _tr_error;
	tools::settings::CSettingsLoader* _settings_module;

	std::string _device_port;

	std::string _server_port;
	std::string _server_address;

	void fill_settings_values();

public:
	CCommonSettings();
	virtual ~CCommonSettings();

	bool ReadSettings();

	std::string GetDevicePort() const;

	std::string GetServerAddress() const;

	std::string GetServerPort() const;

};

}