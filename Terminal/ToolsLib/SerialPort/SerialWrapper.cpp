#include "stdafx.h"
#include "SerialWrapper.h"


void tools::serial_port::CSerialWrapper::thread_method()
{
	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		receive_data();

		send_data();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	cleanup();
	_thread_running = false;
}

tools::serial_port::e_serial_result tools::serial_port::CSerialWrapper::init()
{
	if (tools::e_init_state::was_init == _init_state)
		return e_serial_result::success;

	_init_state = tools::e_init_state::not_init;

	ZeroMemory(_read_buffer, sizeof(_read_buffer));
	ZeroMemory(_write_buffer, sizeof(_write_buffer));

	std::wstring port_name = _T("\\\\.\\") + _connection_params.port_name;
	if (ERROR_SUCCESS != _serial_port.Open(port_name.c_str(), sizeof(_read_buffer), sizeof(_write_buffer)))
		return e_serial_result::error;

	LONG serial_connection_result = _serial_port.Setup(static_cast<CSerial::EBaudrate>(_connection_params.baud_rate),
													   static_cast<CSerial::EDataBits>(_connection_params.bits_per_byte),
													   CSerial::EParNone,
													   static_cast<CSerial::EStopBits>(_connection_params.stop_bits));

	if (ERROR_SUCCESS != serial_connection_result)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(serial_connection_result));
		_tr_error->trace_error(_T("Ошибка подключения к COM порту."));

		return e_serial_result::error;
	}

	if (ERROR_SUCCESS != _serial_port.Purge())
		return e_serial_result::error;

	if (ERROR_SUCCESS != _serial_port.GetError())
		return e_serial_result::error;

	if (ERROR_SUCCESS != _serial_port.SetupHandshaking(CSerial::EHandshakeOff))
		return e_serial_result::error;

	_work_loop_status = tools::e_work_loop_status::ok;
	_thread_running = true;
	_this_thread = std::thread(&CSerialWrapper::thread_method, this);
	_init_state = tools::e_init_state::was_init;
	return e_serial_result::success;
}

void tools::serial_port::CSerialWrapper::cleanup()
{
	_serial_port.Close();

	_this_thread.detach();

	_init_state = tools::e_init_state::not_init;

}

void tools::serial_port::CSerialWrapper::clear_buffers()
{
	if (false == _data_to_send->empty())
	{
		for (data_wrappers::_tag_data_const data : _data_to_send->get_with_cleanup())
		{
			data.free_data();
		}
	}
}

tools::serial_port::e_serial_result tools::serial_port::CSerialWrapper::receive_data()
{
	DWORD readed_count = 0;
	LONG err = _serial_port.Read(_read_buffer, sizeof(_read_buffer), &readed_count);

	if (ERROR_SUCCESS != err)
		return e_serial_result::error;

	if (0 == readed_count)
		return e_serial_result::no_data;

	data_wrappers::_tag_data_managed received_data(_read_buffer, readed_count);
	_received_data.push_back(received_data);

	if (_on_data_received)
		_on_data_received(received_data);

	return e_serial_result::success;
}

tools::serial_port::e_serial_result tools::serial_port::CSerialWrapper::send_data()
{
	if (true == _data_to_send->empty())
		return e_serial_result::success;

	std::vector<data_wrappers::_tag_data_const> data_collection = _data_to_send->get_with_cleanup();

	for (data_wrappers::_tag_data_const packet : data_collection)
	{
		DWORD bytes_written = 0;
		LONG err = _serial_port.Write(packet.p_data, packet.data_size, &bytes_written);
		packet.free_data();

		if (ERROR_SUCCESS != err)
		{
			_tr_error->trace_error(_T("ошибка при отправке данных в COM-порт"));
			_tr_error->trace_error(_tr_error->format_sys_message(::GetLastError()));
			return e_serial_result::error;
		}
		if (0 == bytes_written)
		{
			_tr_error->trace_error(_tr_error->format_sys_message(::GetLastError()));
			_tr_error->trace_error(_T("0 == bytes_written"));
			return e_serial_result::error;
		}
	}
	return e_serial_result::success;
}

tools::serial_port::CSerialWrapper::CSerialWrapper(tools::lock_vector<data_wrappers::_tag_data_managed>& received_data,
												   std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received)
												   : _received_data(received_data)
												   , _init_state(tools::e_init_state::not_init)
												   , _thread_running(false)
												   , _on_data_received(on_data_received)
												   , _work_loop_status(tools::e_work_loop_status::stop)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	_data_to_send = std::make_shared<tools::lock_deque<data_wrappers::_tag_data_const>>();
}

tools::serial_port::CSerialWrapper::~CSerialWrapper()
{
}

tools::serial_port::e_serial_result tools::serial_port::CSerialWrapper::Start(const tag_connection_params& params)
{
	if (tools::e_init_state::was_init == _init_state)
		return e_serial_result::was_connected;

	_connection_params = params;

	if (e_serial_result::error == init())
		return e_serial_result::error;

	return e_serial_result::success;
}

void tools::serial_port::CSerialWrapper::Stop()
{
	clear_buffers();

	if (tools::e_init_state::not_init == _init_state)
		return;

	_work_loop_status = tools::e_work_loop_status::stop;

	while (true == _thread_running)
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void tools::serial_port::CSerialWrapper::PushBackToSend(data_wrappers::_tag_data_const data)
{
	data_wrappers::_tag_data_managed man_data(data);
	man_data.free_after_destruct = false;
	_data_to_send->push_back(static_cast<data_wrappers::_tag_data_const>(man_data));
}

void tools::serial_port::CSerialWrapper::PushFrontToSend(data_wrappers::_tag_data_const data)
{
	data_wrappers::_tag_data_managed man_data(data);
	man_data.free_after_destruct = false;
	_data_to_send->push_front(static_cast<data_wrappers::_tag_data_const>(man_data));
}
