#include "stdafx.h"
#include "DeviceInteract.h"
#include "tools.h"
#include "SerialUtils.h"


void device_exchange::CDeviceInteract::thread_fn()
{
	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		device_to_logic();

		logic_to_device();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void device_exchange::CDeviceInteract::device_to_logic()
{
	std::vector<tools::data_wrappers::_tag_data_managed> packets_to_logic = _device_raw_data.get_with_cleanup();

	for (tools::data_wrappers::_tag_data_const data : packets_to_logic)
	{
		device_exchange::tag_packet_from_device packet;
		if (device_exchange::e_convert_result::success == _device_packet_parser.Parse(data, packet))
		{
			_packets_to_logic.push_back(_packet_from_device_to_packet_to_logic.Convert(packet, packet.command));
		}
		else
		{
			_tr_error->trace_error(_T("Ќе удалось распарсить пакет от устройства"));
			_tr_error->trace_error(tools::binary_to_hex(data));
		}
	}
}

void device_exchange::CDeviceInteract::logic_to_device()
{
	std::vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>> packets_to_device = _packets_to_device.get_with_cleanup();

	for (std::shared_ptr<logic_structures::tag_base_data_from_pc> packet_to_device : packets_to_device)
	{
		tools::data_wrappers::_tag_data_managed raw_data;
		tag_packet_from_pc packet = _packets_to_device_to_packets_from_logic.Convert(packet_to_device, packet_to_device->command_id);

		if (device_exchange::e_convert_result::success == _device_packet_creator.CreateRawData(packet, raw_data))
		{
			_device_connection.PushBackToSend(raw_data);
		}
	}
}

tools::e_init_state device_exchange::CDeviceInteract::init()
{
	tools::serial_port::tag_connection_params connection_params;

	connection_params.port_name = _settings_module.GetDevicePortName();

	if (tools::serial_port::e_serial_result::error == _device_connection.Start(connection_params))
		return tools::e_init_state::not_init;

	_work_loop_status = tools::e_work_loop_status::ok;
	_this_thread = std::thread(&CDeviceInteract::thread_fn, this);
	_init_state = tools::e_init_state::was_init;

	return _init_state;
}

device_exchange::CDeviceInteract::CDeviceInteract(const logic_settings::CCommonSettings& settings_module,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& packets_to_logic,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& packets_to_device)
												  : _packets_to_device(packets_to_device)
												  , _packets_to_logic(packets_to_logic)
												  , _settings_module(settings_module)
												  , _device_connection(_device_raw_data, nullptr)
												  , _work_loop_status(tools::e_work_loop_status::stop)
												  , _init_state(tools::e_init_state::not_init)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

device_exchange::CDeviceInteract::~CDeviceInteract()
{
}

bool device_exchange::CDeviceInteract::Start()
{
	if (tools::e_init_state::was_init == _init_state)
		return true;

	return tools::e_init_state::was_init == init();
}

void device_exchange::CDeviceInteract::Stop()
{
	_work_loop_status = tools::e_work_loop_status::stop;

	if (true == _this_thread.joinable())
		_this_thread.join();

	_device_connection.Stop();

	_init_state = tools::e_init_state::not_init;
}

bool device_exchange::CDeviceInteract::IsStarted()
{
	return ((tools::e_init_state::was_init == _init_state) && (tools::e_work_loop_status::ok == _work_loop_status));
}
