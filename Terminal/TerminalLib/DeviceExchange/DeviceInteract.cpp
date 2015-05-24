#include "stdafx.h"
#include "DeviceInteract.h"
#include "tools.h"
#include "SerialUtils.h"


void device_exchange::CDeviceInteract::on_timer(uint32_t)
{
	time_t send_packet_time = get_send_packet_time();

	if (0 == send_packet_time)
		return;

	time_t current_time = std::time(0);

	if ((current_time - send_packet_time) > 3)
	{
		if (_on_device_not_available_fn)
			_on_device_not_available_fn();

		_timer.stop();
	}
}

void device_exchange::CDeviceInteract::set_send_packet_time(time_t t)
{
	Concurrency::critical_section::scoped_lock sl(_check_device_cs);

	_send_packet_time = t;
}

time_t device_exchange::CDeviceInteract::get_send_packet_time()
{
	Concurrency::critical_section::scoped_lock sl(_check_device_cs);
	
	return _send_packet_time;
}

void device_exchange::CDeviceInteract::thread_fn()
{
	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		device_to_logic();

		logic_to_device();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	_timer.stop();
}

void device_exchange::CDeviceInteract::device_to_logic()
{
	std::vector<tools::data_wrappers::_tag_data_managed> packets_to_logic = _device_raw_data.get_with_cleanup();

	for (tools::data_wrappers::_tag_data_const data : packets_to_logic)
	{
		std::vector<device_exchange::tag_packet_from_device> packets;
		if (device_exchange::e_convert_result::success == _device_packet_parser.Parse(data, packets))
		{
			for (device_exchange::tag_packet_from_device packet : packets)
			{
				_packets_to_logic.push_back(_packet_from_device_to_packet_to_logic.Convert(packet, packet.command));
			}

			if (0 != get_send_packet_time())
			{
				set_send_packet_time(0);
			}
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

			if (true == packet_to_device->need_answer)
			{
				set_send_packet_time(std::time(0));
			}
		}
	}
}

tools::e_init_state device_exchange::CDeviceInteract::init()
{
	tools::serial_port::tag_connection_params connection_params;

	connection_params.port_name = _settings_module.GetDevicePortName();

	if (tools::serial_port::e_serial_result::error == _device_connection.Start(connection_params))
	{
		if (_on_device_not_available_fn)
			_on_device_not_available_fn();

		return tools::e_init_state::not_init;
	}

	_work_loop_status = tools::e_work_loop_status::ok;
	_this_thread = std::thread(&CDeviceInteract::thread_fn, this);
	_timer.start();
	_init_state = tools::e_init_state::was_init;

	return _init_state;
}

device_exchange::CDeviceInteract::CDeviceInteract(const logic_settings::CCommonSettings& settings_module,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_device>>& packets_to_logic,
												  tools::lock_vector<std::shared_ptr<logic_structures::tag_base_data_from_pc>>& packets_to_device,
												  std::function<void(void)> on_device_not_available_fn)
												  : _packets_to_device(packets_to_device)
												  , _packets_to_logic(packets_to_logic)
												  , _settings_module(settings_module)
												  , _device_connection(_device_raw_data, nullptr)
												  , _work_loop_status(tools::e_work_loop_status::stop)
												  , _init_state(tools::e_init_state::not_init)
												  , _on_device_not_available_fn(on_device_not_available_fn)
												  , _send_packet_time(0)
												  , _timer(5000, 0, &_on_timer_call, true)
												  , _on_timer_call(std::bind(std::mem_fn(&CDeviceInteract::on_timer), this, std::placeholders::_1))
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
