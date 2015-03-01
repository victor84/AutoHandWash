#include "stdafx.h"
#include "AsyncClientSocket.h"
#include "tools.h"

using namespace tools;
using namespace tools::networking;

CAsyncClientSocket::CAsyncClientSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data)
									   : _received_data(received_data)
									   , _reconnection_timer(nullptr)
									   , _reconnection_method([this](INT)
																{
																	if (_e_connection_state::not_connected == _connection_state)
																		OpenConnection(_connection_params);
																})
										
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

CAsyncClientSocket::~CAsyncClientSocket()
{
	CloseConnection();
}

e_socket_result CAsyncClientSocket::OpenConnection(const tag_connection_params& params)
{
	if (_e_connection_state::connected == _connection_state)
		return e_socket_result::was_connected;

	_connection_params = params;

	if (e_socket_result::success != init())
		return e_socket_result::error;

	pause_reconnection_timer();

	_socket_fn_result = ::connect(_client_socket, _addrptr->ai_addr, _addrptr->ai_addrlen);

	if (false == check_socket_fn_result_not(SOCKET_ERROR))
	{
		_str_str.str(_T(""));
		_str_str << _T("Не удалось подключиться к хосту: ") << _connection_params.address.c_str()
			<< _T(" на порт ") << _connection_params.port.c_str()
			<< _T(" ошибка: ") << _tr_error->format_sys_message(WSAGetLastError());

		_tr_error->trace_error(_str_str.str());

		_connection_state = _e_connection_state::not_connected;

		start_reconnection_timer();

		return e_socket_result::error;
	}

	_connection_state = _e_connection_state::connected;

	_work_loop_status = _e_work_loop_status::ok;
	_this_thread = std::thread(&CAsyncClientSocket::thread_method, this);

	return e_socket_result::success;
}

e_socket_result CAsyncClientSocket::init()
{
	if (_e_init_state::was_init == _init_state)
		return e_socket_result::success;

	_init_state = _e_init_state::not_init;

	ZeroMemory(&_addr_hints, sizeof(_addr_hints));

	WSADATA wsa_data;
	ZeroMemory(&wsa_data, sizeof(wsa_data));

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
		return e_socket_result::error;
	}

#define cleanup_and_return_error	 { cleanup(); return e_socket_result::error; }

	_addr_hints.ai_family = AF_INET;
	_addr_hints.ai_socktype = SOCK_STREAM;
	_addr_hints.ai_protocol = IPPROTO_TCP;

	_socket_fn_result = ::getaddrinfo(_connection_params.address.c_str(), 
									  _connection_params.port.c_str(), 
									  &_addr_hints, 
									  &_addr_results);
	if (FALSE == check_socket_fn_result_and(0))
		cleanup_and_return_error;

	if (nullptr == _addr_results)
	{
		_str_str.str(_T(""));
		_str_str << _T("Unable to resolve interface ") << tools::string_to_wstring(_connection_params.port);
		_tr_error->trace_error(_str_str.str());
		cleanup_and_return_error;
	}

	_addrptr = _addr_results;
	_client_socket = ::socket(_addrptr->ai_family, _addrptr->ai_socktype, _addrptr->ai_protocol);
	if (_client_socket == INVALID_SOCKET)
	{
		_str_str.str(_T(""));
		_str_str << _T("Не удалось создать сокет. Ошибка ") << _tr_error->format_sys_message(WSAGetLastError());
		_tr_error->trace_error(_str_str.str());
		cleanup_and_return_error;
	}

	return e_socket_result::success;
}

void CAsyncClientSocket::cleanup()
{
	if (nullptr != _addr_results)
	{
		freeaddrinfo(_addr_results);
		_addr_results = nullptr;
	}

	if (INVALID_SOCKET != _client_socket)
	{
		closesocket(_client_socket);
		_client_socket = INVALID_SOCKET;
	}

	::WSACleanup();
}

void CAsyncClientSocket::thread_method()
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
	
	if (_e_work_loop_status::error == _work_loop_status)
	{
		inner_close_connection();
	}
}

bool CAsyncClientSocket::check_socket_fn_result_and(const INT& valid_val)
{
	if (valid_val == _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));

	return false;
}

bool CAsyncClientSocket::check_socket_fn_result_not(const INT& invalid_val)
{
	if (invalid_val != _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));

	return false;
}

e_socket_result networking::CAsyncClientSocket::inner_close_connection()
{
	if (_e_connection_state::not_connected == _connection_state)
		return e_socket_result::was_disconnected;

	// если внезапно =( остановилось или оборвалось, перезапускаем
	start_reconnection_timer();

	return e_socket_result::success;
}

e_socket_result CAsyncClientSocket::CloseConnection()
{
	if (_e_connection_state::not_connected == _connection_state)
		return e_socket_result::was_disconnected;

	if (nullptr != _reconnection_timer)
	{
		_reconnection_timer->stop();
		delete _reconnection_timer;
		_reconnection_timer = nullptr;
	}

	if (SOCKET_ERROR != _client_socket)
		::closesocket(_client_socket);

	cleanup();
	_connection_state = _e_connection_state::not_connected;

	_work_loop_status = _e_work_loop_status::stop;

	if (_this_thread.joinable())
		_this_thread.join();

	return e_socket_result::success;
}

e_socket_result CAsyncClientSocket::receive_data()
{
	_received_bytes_count = ::recv(_client_socket, _received_buffer, sizeof(_received_buffer), 0);
	if (0 == _received_bytes_count)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));
		_tr_error->trace_error(_T("принято 0 байт"));
		return e_socket_result::error;
	}

	if (SOCKET_ERROR == _received_bytes_count)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));
		_tr_error->trace_error(_T("ошибка при приёме данных из сокета"));
		
		return e_socket_result::error;
	}

	data_wrappers::_tag_data_managed received_data(_received_buffer, _received_bytes_count);

	_received_data.push_back(received_data);

	return e_socket_result::success;
}

CAsyncClientSocket::_e_check_socket_result CAsyncClientSocket::check_socket(const _e_check_socket_type& cst)
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
		case _e_check_socket_type::read:	sock_set_read	= &sock_set;	break;
		case _e_check_socket_type::write:	sock_set_write	= &sock_set;	break;
		default: return _e_check_socket_result::error;
	}

	result = ::select(0, sock_set_read, sock_set_write, nullptr, &wait_time);
	if (SOCKET_ERROR == result)
	{
		_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));
		return _e_check_socket_result::error;
	}

	return (0 == result) ? _e_check_socket_result::not_ready : _e_check_socket_result::ready;
}

e_socket_result CAsyncClientSocket::send_data()
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
			_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));

			return e_socket_result::error;
		}

		if (0 == result)
		{
			_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));
			_tr_error->trace_error(_T("0 == result"));
			return e_socket_result::error;
		}
	}

	return e_socket_result::success;
}

void CAsyncClientSocket::start_reconnection_timer()
{
	if (0 == _connection_params.reconnection_timeout)
		return;
	
	if (nullptr == _reconnection_timer)
	{
		_reconnection_timer = new Concurrency::timer<INT>(_connection_params.reconnection_timeout,
														    1,
															&_reconnection_method,
															true);
	}
	_reconnection_timer->start();
}

void CAsyncClientSocket::pause_reconnection_timer()
{
	if (nullptr == _reconnection_timer)
		return;

	_reconnection_timer->pause();
}

void CAsyncClientSocket::Send(data_wrappers::_tag_data_const data)
{
	data_wrappers::_tag_data_managed man_data(data);
	man_data.free_after_destruct = false;
	_data_to_send.push_back(static_cast<data_wrappers::_tag_data_const>(man_data));
}

