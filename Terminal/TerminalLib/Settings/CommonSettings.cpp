#include "stdafx.h"
#include "CommonSettings.h"


void logic_settings::CCommonSettings::fill_settings_values()
{
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
									device_block_name, port_param_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
									server_block_name, port_param_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
									server_block_name, address_param_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
									terminal_block_name, terminal_name_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_string,
									terminal_block_name, terminal_group_name);

}

logic_settings::CCommonSettings::CCommonSettings()
{
	_tr_error = tools::logging::CTraceError::get_instance();
	_settings_module = tools::settings::CSettingsLoader::get_instance();

	fill_settings_values();
}

logic_settings::CCommonSettings::~CCommonSettings()
{
}

bool logic_settings::CCommonSettings::ReadSettings()
{
	if (FALSE == _settings_module->read(port_param_name, device_block_name))
		return false;

	if (FALSE == _settings_module->read(port_param_name, server_block_name))
		return false;

	if (FALSE == _settings_module->read(address_param_name, server_block_name))
		return false;

	_device_port = CStringA(_settings_module->get_string(port_param_name, device_block_name)).GetString();
	_server_port = CStringA(_settings_module->get_string(port_param_name, server_block_name)).GetString();
	_server_address = CStringA(_settings_module->get_string(address_param_name, server_block_name)).GetString();

	_terminal_name = _settings_module->get_string(terminal_name_name, terminal_block_name);
	_terminal_group = _settings_module->get_string(terminal_group_name, terminal_block_name);

	return true;
}

std::string logic_settings::CCommonSettings::GetDevicePort() const
{
	return _device_port;
}

std::string logic_settings::CCommonSettings::GetServerAddress() const
{
	return _server_address;
}

std::string logic_settings::CCommonSettings::GetServerPort() const
{
	return _server_address;
}

std::wstring logic_settings::CCommonSettings::GetTerminalName() const
{
	return _terminal_name;
}

std::wstring logic_settings::CCommonSettings::GetTerminalGroup() const
{
	return _terminal_group;
}
