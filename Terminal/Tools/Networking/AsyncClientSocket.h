#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"

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
	// ������ ���������� ��������� �������� �����
	enum class _e_work_loop_status
	{
		ok,		// ���� ������������
		error,	// �������� ������
		stop	// ������� ������������ �������
	};

	// ��������� ����������
	enum class _e_connection_state
	{
		not_connected,
		connected
	};

	// ��������� �������������
	enum class _e_init_state
	{
		not_init,
		was_init
	};

	// ��� �������� ������ �� ����������
	enum class _e_check_socket_type
	{
		read,	// ������
		write	// ������
	};

	// ��������� �������� ������
	enum class _e_check_socket_result
	{
		error,		// ������ ��� ��������
		ready,		// ����� ����� ��� ��������
		not_ready	// ����� �� ����� ��� ��������
	};


	tools::logging::CTraceError* _tr_error;

	// ������ � �������� ������
	std::wstringstream _str_str;

	SOCKET		_client_socket;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// ����� ��� ����� ������
	CHAR _received_buffer[0x10000];

	// ���������� �������� ����
	INT _received_bytes_count;

	// ��������� ���������� ������� windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	_e_connection_state _connection_state;
	_e_init_state _init_state;

	// �������� ������
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// ������ ��� ��������
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// ����� ��������� ������
	std::thread _this_thread;

	// ������ ��������� ������
	_e_work_loop_status _work_loop_status;

	void thread_method();

	e_socket_result init();

	void cleanup();

	// true ���� _socket_fn_result ����� valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true ���� _socket_fn_result �� ����� invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	// �������� ������ 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// ���� ������
	e_socket_result receive_data();

	// �������� ������
	e_socket_result send_data();

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


