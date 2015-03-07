#include "stdafx.h"
#include "SocketStream.h"

using namespace tools::networking;
using namespace tools::data_wrappers;

CSocketStream::CSocketStream(tools::lock_vector<_tag_data_const>& received_data)
	: _received_data(received_data)
	, _start_state(_e_init_state::not_init)
	, _thread_running(false)
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

CSocketStream::~CSocketStream()
{
}

e_socket_result CSocketStream::Start(const SOCKET& socket_to_process,
									 _e_work_loop_status* end_status,
									 std::function<void()> on_complete_fn)
{
	if (_e_init_state::was_init == _start_state)
		return e_socket_result::was_connected;

	_client_socket = socket_to_process;
	_on_complete_fn = on_complete_fn;
	_end_status = end_status;
	_work_loop_status = _e_work_loop_status::ok;
	_thread_running = true;
	_this_thread = std::thread(&CSocketStream::thread_method, this);
	_start_state = _e_init_state::was_init;
	return e_socket_result::success;
}

e_socket_result CSocketStream::Stop()
{
	if (_e_init_state::not_init == _start_state)
		return e_socket_result::was_disconnected;

	_work_loop_status = _e_work_loop_status::stop;

	while (true == _thread_running)
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	return e_socket_result::success;
}

void CSocketStream::thread_method()
{
	_e_check_socket_result cs_result = _e_check_socket_result::error;

	while (_e_work_loop_status::ok == _work_loop_status)
	{
		cs_result = check_socket(_e_check_socket_type::read);
		if (_e_check_socket_result::error == cs_result)
		{
			_work_loop_status = _e_work_loop_status::error;
			break;
		}
		if (_e_check_socket_result::ready == cs_result)
		{
			e_socket_result receive_result = receive_data();
			if (e_socket_result::error == receive_result)
			{
				_work_loop_status = _e_work_loop_status::error;
				break;
			}
		}
		cs_result = check_socket(_e_check_socket_type::write);
		if (_e_check_socket_result::error == cs_result)
		{
			_work_loop_status = _e_work_loop_status::error;
			break;
		}
		if ((_e_check_socket_result::ready == cs_result) && (false == _data_to_send.empty()))
		{
			if (e_socket_result::error == send_data())
			{
				_work_loop_status = _e_work_loop_status::error;
				break;
			}
		}
		Concurrency::wait(500);
	}

	*_end_status = _work_loop_status;
	cleanup();
	_on_complete_fn();
	_thread_running = false;
}

void CSocketStream::cleanup()
{
	_this_thread.detach();

	::closesocket(_client_socket);
	_client_socket = INVALID_SOCKET;

	_start_state = _e_init_state::not_init;
}

CSocketStream::_e_check_socket_result CSocketStream::check_socket(const _e_check_socket_type& cst)
{
	INT result = 0;
	fd_set* sock_set_read = nullptr;
	fd_set* sock_set_write = nullptr;

	fd_set	sock_set;
	FD_ZERO(&sock_set);
	FD_SET(_client_socket, &sock_set);

	timeval	wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_usec = 5000;

	switch (cst)
	{
		case _e_check_socket_type::read:	sock_set_read = &sock_set;	break;
		case _e_check_socket_type::write:	sock_set_write = &sock_set;	break;
		default: return _e_check_socket_result::error;
	}

	result = ::select(0, sock_set_read, sock_set_write, nullptr, &wait_time);
	if (SOCKET_ERROR == result)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
		return _e_check_socket_result::error;
	}

	return (0 == result) ? _e_check_socket_result::not_ready : _e_check_socket_result::ready;
}

e_socket_result CSocketStream::receive_data()
{
	_received_bytes_count = ::recv(_client_socket, _received_buffer, sizeof(_received_buffer), 0);
	if (0 == _received_bytes_count)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
		_tr_error->trace_error(_T("принято 0 байт"));
		return e_socket_result::error;
	}

	if (SOCKET_ERROR == _received_bytes_count)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
		_tr_error->trace_error(_T("ошибка при приёме данных из сокета"));

		return e_socket_result::error;
	}

	data_wrappers::_tag_data_managed received_data(_received_buffer, _received_bytes_count);
	_received_data.push_back(received_data);

	return e_socket_result::success;
}

e_socket_result CSocketStream::send_data()
{
	if (true == _data_to_send.empty())
		return e_socket_result::success;

	std::vector<data_wrappers::_tag_data_const> data_collection = _data_to_send.get_with_cleanup();

	for (data_wrappers::_tag_data_const packet : data_collection)
	{
		INT result = ::send(_client_socket, reinterpret_cast<PCSTR>(packet.p_data), packet.data_size, 0);
		packet.free_data();

		if (SOCKET_ERROR == result)
		{
			_tr_error->trace_error(_T("ошибка при отправке данных в сокет"));
			_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
			return e_socket_result::error;
		}
		if (0 == result)
		{
			_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
			_tr_error->trace_error(_T("0 == result"));
			return e_socket_result::error;
		}
	}

	return e_socket_result::success;
}

void tools::networking::CSocketStream::Send(data_wrappers::_tag_data_const data)
{
	data_wrappers::_tag_data_managed man_data(data);
	man_data.free_after_destruct = false;
	_data_to_send.push_back(static_cast<data_wrappers::_tag_data_const>(man_data));
}
