#pragma once
#include "state.h"
#include "TraceError.h"

namespace logic
{
class CSettingsWorkState : public IState
{
	tools::logging::CTraceError* _tr_error;

	tag_device_settings _settings;
	tag_device_settings _settings_from_device;
	uint32_t* _p_settings;

	std::wstringstream _str_str;

	byte _write_cell_number;
	byte _write_setting_number;

public:
	CSettingsWorkState(CLogicAbstract& logic);
	virtual ~CSettingsWorkState();

	virtual void refilled_cache() final;

	virtual void service_button_press(e_service_name service_name);

	virtual void stop_button_press() final;

	virtual void time_out() final;

	virtual void out_of_money() final;

	virtual void device_confirm() final;

	void data_from_eeprom(byte cell_number, uint32_t value);

	void read_settings();

	tag_device_settings get_settings() const;

	void set_settings(const tag_device_settings& settings);

	void write_settings();
};
}



