#include "stdafx.h"
#include "SingleServerSocket.h"
#include "tools.h"


using namespace tools::networking;

CSingleServerSocket::CSingleServerSocket(tools::lock_vector<tools::data_wrappers::_tag_data_const>& received_data)
										 : _received_data(received_data)
										 , _can_new_accept(true)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	ZeroMemory(&_addr_hints, sizeof(_addr_hints));
}


CSingleServerSocket::~CSingleServerSocket()
{
}

e_socket_result CSingleServerSocket::init()
{
	if (_init_state == _e_init_state::was_init)
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

	_init_state = _e_init_state::was_init;

	return e_socket_result::success;
}

bool CSingleServerSocket::check_socket_fn_result_and(const INT& valid_val)
{
	if (valid_val == _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));

	return false;
}

bool CSingleServerSocket::check_socket_fn_result_not(const INT& invalid_val)
{
	if (invalid_val != _socket_fn_result)
		return true;

	_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));

	return false;
}

void CSingleServerSocket::cleanup()
{
	if (nullptr != _addr_results)
	{
		freeaddrinfo(_addr_results);
		_addr_results = nullptr;
	}

	if (INVALID_SOCKET != _client_socket)
	{
		::shutdown(_client_socket, SD_BOTH);
		::closesocket(_client_socket);
		_client_socket = INVALID_SOCKET;
	}

	if (INVALID_SOCKET != _listen_socket)
	{
		::closesocket(_listen_socket);
		_listen_socket = INVALID_SOCKET;
	}
}

e_socket_result CSingleServerSocket::Start(const tag_connection_params& params)
{
	_connection_params = params;
	init();

	_listen_thread = std::thread(&CSingleServerSocket::listen_method, this);

	return e_socket_result::success;
}

void tools::networking::CSingleServerSocket::listen_method()
{
	while (_work_loop_status == _e_work_loop_status::ok)
	{
		while (false == _can_new_accept)
			Concurrency::wait(500);

		SOCKADDR_STORAGE	sock_address;
		INT					sock_address_len = sizeof(sock_address);
		CHAR				servstr[NI_MAXSERV];
		CHAR				hoststr[NI_MAXHOST];
		_client_socket = INVALID_SOCKET;

		ZeroMemory(&sock_address, sizeof(sock_address));
		ZeroMemory(&servstr, sizeof(servstr));
		ZeroMemory(&hoststr, sizeof(hoststr));

		_client_socket = ::accept(_listen_socket, (SOCKADDR*)&sock_address, &sock_address_len);

		_socket_fn_result = static_cast<INT>(_client_socket);
		if (FALSE == check_socket_fn_result_not(INVALID_SOCKET))
		{
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
		_str_str << _T("соединение с хостом: ") << hoststr
			<< _T(" удалённый порт: ") << servstr;
		_tr_error->trace_message(_str_str.str());
		_can_new_accept = false;

		if (true == _client_thread.joinable())
			_client_thread.join();
		_client_thread = std::thread(&CSingleServerSocket::client_method, this);
	}
}

void tools::networking::CSingleServerSocket::client_method()
{
	_e_check_socket_result cs_result = _e_check_socket_result::error;

	while (true)
	{
		cs_result = check_socket(_e_check_socket_type::read);
		if (_e_check_socket_result::error == cs_result)
			break;

		if (_e_check_socket_result::ready == cs_result)
		{
			e_socket_result receive_result = receive_data();
			if (e_socket_result::error == receive_result)
				break;
		}

		cs_result = check_socket(_e_check_socket_type::write);
		if (_e_check_socket_result::error == cs_result)
			break;

		if ((_e_check_socket_result::ready == cs_result) && (false == _data_to_send.empty()))
		{
			if (e_socket_result::error == send_data())
				break;
		}

		Concurrency::wait(500);
	}

	_tr_error->trace_message(_T("Клиент отключился"));

	::shutdown(_client_socket, SD_BOTH);
	::closesocket(_client_socket);
	_client_socket = INVALID_SOCKET;

	_can_new_accept = true;
}

e_socket_result CSingleServerSocket::Stop()
{
	_work_loop_status = _e_work_loop_status::stop;

	cleanup();

	if (_listen_thread.joinable())
		_listen_thread.join();

	if (_client_thread.joinable())
		_client_thread.join();

	_init_state = _e_init_state::not_init;

	return e_socket_result::success;
}

e_socket_result CSingleServerSocket::receive_data()
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

CSingleServerSocket::_e_check_socket_result CSingleServerSocket::check_socket(const _e_check_socket_type& cst)
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
		_tr_error->trace_error(_tr_error->format_sys_message(WSAGetLastError()));
		return _e_check_socket_result::error;
	}

	return (0 == result) ? _e_check_socket_result::not_ready : _e_check_socket_result::ready;
}

e_socket_result CSingleServerSocket::send_data()
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

