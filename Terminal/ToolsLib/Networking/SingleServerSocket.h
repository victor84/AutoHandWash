#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"


/*!
 * \file SingleServerSocket.h
 *
 * \author Victor Mulika
 * \date 06 ���� 2015
 *
 * 
 */



namespace tools
{
namespace networking
{
class CSingleServerSocket
{


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

	SOCKET	_listen_socket;
	SOCKET	_client_socket;

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

	_e_init_state		_init_state;

	// �������� ������
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// ������ ��� ��������
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// ����� ���������� ����� ������
	std::thread _listen_thread;

	// ����� ����� ������
	std::thread _client_thread;

	// ������ ��������� ������
	_e_work_loop_status _work_loop_status;

	// ����, ��� ����� ��������� ����� ����������
	bool _can_new_accept;

	// ����� ������ ���������� �����
	void listen_method();

	// ����� ������������ ������
	void client_method();

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

	CSingleServerSocket(const CSingleServerSocket& ob);

public:
	CSingleServerSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CSingleServerSocket();


	void Send(data_wrappers::_tag_data_const data);

	// ������� ���������� � ���������� �����������
	e_socket_result Start(const tag_connection_params& params);

	// �������� ������������� �����������
	e_socket_result Stop();
};

}
}