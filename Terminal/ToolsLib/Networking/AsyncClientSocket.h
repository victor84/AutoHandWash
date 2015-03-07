#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"
#include "SocketStream.h"

/*!
 * \file AsyncClientSocket.h
 *
 * \author Victor Mulika
 * \date 28 ������� 2015
 *
 * 
 */

namespace tools
{

namespace networking
{
// ����������� ����� ������
class CAsyncClientSocket
{
	tools::logging::CTraceError* _tr_error;

	SOCKET _client_socket;

	CSocketStream _socket_stream;

	// ������ � �������� ������
	std::wstringstream _str_str;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// ��������� ���������� ������� windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	_e_connection_state _connection_state;
	_e_init_state _init_state;

	// ������ ��������� ������
	_e_work_loop_status _end_of_stream_status;

	e_socket_result init();

	void cleanup();

	// true ���� _socket_fn_result ����� valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true ���� _socket_fn_result �� ����� invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	// ������ ���������������
	Concurrency::timer<INT>* _reconnection_timer;

	// ����� ���������������
	Concurrency::call<INT> _reconnection_method;

	// ������ ������� ���������������
	void start_reconnection_timer();

	//������������� ������ ���������������
	void pause_reconnection_timer();

	// ��������� �� ���������� ��������
	e_socket_result inner_close_connection();

	// ���������� ��� ���������� ������ ������ ��������� ������
	void on_complete_stream_fn();

	CAsyncClientSocket(const CAsyncClientSocket& ob);

public:
	CAsyncClientSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CAsyncClientSocket();

	void Send(data_wrappers::_tag_data_const data);

	// ������� ���������� � ���������� �����������
	e_socket_result OpenConnection(const tag_connection_params& params);

	// �������� ������������� �����������
	e_socket_result CloseConnection();

};
}
}


