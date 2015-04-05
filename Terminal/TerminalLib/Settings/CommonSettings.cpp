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
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, bill_acceptor_impulse_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, coin_acceptor_impulse_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, free_idle_time_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, idle_time_cost_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, pause_before_advertising_name);
	_settings_module->add_parameter(tools::settings::CSettingsLoader::e_parameter_data_type::type_int,
									terminal_block_name, state_name);
}

logic_settings::CCommonSettings::CCommonSettings()
	: _bill_acceptor_impulse(0)
	, _coin_acceptor_impulse(0)
	, _free_idle_time(0)
	, _idle_time_cost(0)
	, _pause_before_advertising(0)
	, _state(0)
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

	if (FALSE == _settings_module->read(terminal_name_name, terminal_block_name))
		return false;

	if (FALSE == _settings_module->read(terminal_group_name, terminal_block_name))
		return false;

	_device_port = CStringA(_settings_module->get_string(port_param_name, device_block_name)).GetString();
	_server_port = CStringA(_settings_module->get_string(port_param_name, server_block_name)).GetString();
	_server_address = CStringA(_settings_module->get_string(address_param_name, server_block_name)).GetString();

	_terminal_name = _settings_module->get_string(terminal_name_name, terminal_block_name);
	_terminal_group = _settings_module->get_string(terminal_group_name, terminal_block_name);

	_bill_acceptor_impulse = _settings_module->get_int(bill_acceptor_impulse_name, terminal_block_name);
	_coin_acceptor_impulse = _settings_module->get_int(coin_acceptor_impulse_name, terminal_block_name);
	_free_idle_time = _settings_module->get_int(free_idle_time_name, terminal_block_name);
	_idle_time_cost = _settings_module->get_int(idle_time_cost_name, terminal_block_name);
	_pause_before_advertising = _settings_module->get_int(pause_before_advertising_name, terminal_block_name);
	_state = _settings_module->get_int(state_name, terminal_block_name);

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
	return _server_port;
}

std::wstring logic_settings::CCommonSettings::GetTerminalName() const
{
	return _terminal_name;
}

std::wstring logic_settings::CCommonSettings::GetTerminalGroup() const
{
	return _terminal_group;
}

uint32_t logic_settings::CCommonSettings::GetBillAcceptorImpulse() const
{
	return _bill_acceptor_impulse;
}

uint32_t logic_settings::CCommonSettings::GetCoinAcceptorImpulse() const
{
	return _coin_acceptor_impulse;
}

uint32_t logic_settings::CCommonSettings::GetFreeIdleTime() const
{
	return _free_idle_time;
}

uint32_t logic_settings::CCommonSettings::GetIdleTimeCost() const
{
	return _idle_time_cost;
}

uint32_t logic_settings::CCommonSettings::GetPauseBeforeAdvertising() const
{
	return _pause_before_advertising;
}

uint32_t logic_settings::CCommonSettings::GetState() const
{
	return _state;
}
