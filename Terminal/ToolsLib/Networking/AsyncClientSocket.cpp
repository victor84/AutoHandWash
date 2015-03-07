#include "stdafx.h"
#include "AsyncClientSocket.h"
#include "tools.h"

using namespace tools;
using namespace tools::networking;

CAsyncClientSocket::CAsyncClientSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data)
									   : _reconnection_timer(nullptr)
									   , _socket_stream(received_data)
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
			<< _T(" ошибка: ") << _tr_error->format_sys_message(::WSAGetLastError());

		_tr_error->trace_error(_str_str.str());
		_connection_state = _e_connection_state::not_connected;
		start_reconnection_timer();
		return e_socket_result::error;
	}

	_connection_state = _e_connection_state::connected;
	_end_of_stream_status = _e_work_loop_status::ok;
	_socket_stream.Start(_client_socket,
						 &_end_of_stream_status,
						 std::bind(std::mem_fn(&CAsyncClientSocket::on_complete_stream_fn), this));

	return e_socket_result::success;
}

void tools::networking::CAsyncClientSocket::on_complete_stream_fn()
{
	if (_e_work_loop_status::error == _end_of_stream_status)
		inner_close_connection();

	_connection_state = _e_connection_state::not_connected;
}

e_socket_result CAsyncClientSocket::inner_close_connection()
{
	if (_e_connection_state::not_connected == _connection_state)
		return e_socket_result::was_disconnected;

	// если внезапно =( остановилось или оборвалось, перезапускаем
	start_reconnection_timer();
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
		_str_str << _T("Не удалось создать сокет. Ошибка ") << _tr_error->format_sys_message(::WSAGetLastError());
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

	_socket_stream.Stop();

	::WSACleanup();
}

bool CAsyncClientSocket::check_socket_fn_result_and(const INT& valid_val)
{
	if (valid_val == _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));

	return false;
}

bool CAsyncClientSocket::check_socket_fn_result_not(const INT& invalid_val)
{
	if (invalid_val != _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));

	return false;
}

e_socket_result CAsyncClientSocket::CloseConnection()
{
	if (nullptr != _reconnection_timer)
	{
		_reconnection_timer->stop();
		delete _reconnection_timer;
		_reconnection_timer = nullptr;
	}

	cleanup();
	_connection_state = _e_connection_state::not_connected;

	return e_socket_result::success;
}

void CAsyncClientSocket::start_reconnection_timer()
{
	if (0 == _connection_params.reconnection_timeout)
		return;
	
	delete _reconnection_timer;
	_reconnection_timer = new Concurrency::timer<INT>(_connection_params.reconnection_timeout,
														1,
														&_reconnection_method,
														false);
	
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
	_socket_stream.Send(data);
}

