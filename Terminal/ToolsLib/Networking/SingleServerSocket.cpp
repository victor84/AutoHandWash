#include "stdafx.h"
#include "SingleServerSocket.h"
#include "tools.h"

using namespace tools::networking;

CSingleServerSocket::CSingleServerSocket(tools::lock_vector<tools::data_wrappers::_tag_data_managed>& received_data,
										 std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received)
										 : _socket_stream(received_data, on_data_received)
										 , _addr_results(nullptr)
										 , _work_loop_status(tools::e_work_loop_status::stop)
										 , _init_state(tools::e_init_state::not_init)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	ZeroMemory(&_addr_hints, sizeof(_addr_hints));
}


CSingleServerSocket::~CSingleServerSocket()
{
}

e_socket_result CSingleServerSocket::init()
{
	if (_init_state == tools::e_init_state::was_init)
		return e_socket_result::success;

	ZeroMemory(&_addr_hints, sizeof(_addr_hints));

	WSADATA wsa_data;
	ZeroMemory(&wsa_data, sizeof(wsa_data));

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
		return e_socket_result::error;
	}

#define cleanup_and_return_false	 { cleanup(); return e_socket_result::error; }

	_addr_hints.ai_family = AF_INET;
	_addr_hints.ai_socktype = SOCK_STREAM;
	_addr_hints.ai_protocol = IPPROTO_TCP;
	_addr_hints.ai_flags = AI_PASSIVE;

	std::string local_ip_address_str = "127.0.0.1";
	if (TRUE == local_ip_address_str.empty())
	{
		_tr_error->trace_error(_T(""));
		return e_socket_result::error;
	}

	_socket_fn_result = ::getaddrinfo(local_ip_address_str.c_str(), _connection_params.port.c_str(), &_addr_hints, &_addr_results);
	if (FALSE == check_socket_fn_result_and(0))
		cleanup_and_return_false;

	if (nullptr == _addr_results)
	{
		_str_str.str(_T(""));
		_str_str << _T("Unable to resolve interface ") << tools::string_to_wstring(_connection_params.port);
		_tr_error->trace_error(_str_str.str());
		cleanup_and_return_false;
	}

	_addrptr = _addr_results;
	_listen_socket = ::socket(_addrptr->ai_family, _addrptr->ai_socktype, _addrptr->ai_protocol);

	_socket_fn_result = static_cast<INT>(_listen_socket);
	if (FALSE == check_socket_fn_result_not(INVALID_SOCKET))
		cleanup_and_return_false;

	_socket_fn_result = ::bind(_listen_socket, _addrptr->ai_addr, static_cast<INT>(_addrptr->ai_addrlen));
	if (FALSE == check_socket_fn_result_not(SOCKET_ERROR))
		cleanup_and_return_false;

	_socket_fn_result = ::listen(_listen_socket, 10);
	if (FALSE == check_socket_fn_result_not(SOCKET_ERROR))
		cleanup_and_return_false;

	_init_state = tools::e_init_state::was_init;

	return e_socket_result::success;
}

bool CSingleServerSocket::check_socket_fn_result_and(const INT& valid_val)
{
	if (valid_val == _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));

	return false;
}

bool CSingleServerSocket::check_socket_fn_result_not(const INT& invalid_val)
{
	if (invalid_val != _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));

	return false;
}

void CSingleServerSocket::cleanup()
{
	if (nullptr != _addr_results)
	{
		freeaddrinfo(_addr_results);
		_addr_results = nullptr;
	}

	if (INVALID_SOCKET != _listen_socket)
	{
		::closesocket(_listen_socket);
		_listen_socket = INVALID_SOCKET;
	}

	_socket_stream.Stop();
}

e_socket_result CSingleServerSocket::Start(const tag_connection_params& params)
{
	_connection_params = params;
	init();

	_work_loop_status = e_work_loop_status::ok;
	_listen_thread = std::thread(&CSingleServerSocket::listen_method, this);

	return e_socket_result::success;
}

void tools::networking::CSingleServerSocket::listen_method()
{
	while (_work_loop_status == e_work_loop_status::ok)
	{
		SOCKADDR_STORAGE	sock_address;
		INT					sock_address_len = sizeof(sock_address);
		CHAR				servstr[NI_MAXSERV];
		CHAR				hoststr[NI_MAXHOST];
		SOCKET				client_socket = INVALID_SOCKET;

		ZeroMemory(&sock_address, sizeof(sock_address));
		ZeroMemory(&servstr, sizeof(servstr));
		ZeroMemory(&hoststr, sizeof(hoststr));

		client_socket = ::accept(_listen_socket, (SOCKADDR*)&sock_address, &sock_address_len);

		_socket_fn_result = static_cast<INT>(client_socket);
		if (FALSE == check_socket_fn_result_not(INVALID_SOCKET))
		{
			_tr_error->trace_error(_tr_error->format_sys_message(::WSAGetLastError()));
			_tr_error->trace_error(_T("accept error"));
			continue;
		}

		_socket_fn_result = getnameinfo((SOCKADDR*)&sock_address,
										 sock_address_len,
										 hoststr,
										 _countof(hoststr),
										 servstr,
										 _countof(servstr),
										 NI_NUMERICHOST | NI_NUMERICSERV);
		if (FALSE == check_socket_fn_result_and(NULL))
		{
			_tr_error->trace_error(_T("getnameinfo error"));
			continue;
		}

		_str_str.str(_T(""));
		_str_str << _T("Cоединение с хостом: ") << hoststr
					<< _T(" удалённый порт: ") << servstr;
		_tr_error->trace_message(_str_str.str());

		_socket_stream.Start(client_socket,
							 &_stream_end_status,
							 std::bind(std::mem_fn(&CSingleServerSocket::on_complete_stream_fn), this));

		// ожидание завершения потока сокета
		_lock_wait_stream = new std::unique_lock<std::mutex>(_wait_stream_mutex);
		
		while (e_work_loop_status::ok == _stream_end_status)
			_cv.wait(*_lock_wait_stream);

		delete _lock_wait_stream;
		_lock_wait_stream = nullptr;
	}
}
void CSingleServerSocket::PushBackToSend(tools::data_wrappers::_tag_data_const data)
{
	_socket_stream.PushBackToSend(data);
}

void CSingleServerSocket::PushFrontToSend(tools::data_wrappers::_tag_data_const data)
{
	_socket_stream.PushFrontToSend(data);
}

void CSingleServerSocket::on_complete_stream_fn()
{
	_cv.notify_one();
}

e_socket_result CSingleServerSocket::Stop()
{
	_work_loop_status = e_work_loop_status::stop;

	cleanup();

	if (_listen_thread.joinable())
		_listen_thread.join();

	_init_state = tools::e_init_state::not_init;

	return e_socket_result::success;
}