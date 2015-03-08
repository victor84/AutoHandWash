#pragma once
#include "TraceError.h"
#include "SocketUtils.h"
#include "raw_data_warappers.h"
#include "lock_vector.h"
#include "SocketStream.h"


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
	tools::logging::CTraceError* _tr_error;

	// ������ � �������� ������
	std::wstringstream _str_str;

	SOCKET	_listen_socket;

	addrinfo	_addr_hints;
	addrinfo	*_addr_results;
	addrinfo	*_addrptr;

	// ��������� ���������� ������� windows socket
	INT _socket_fn_result;

	tag_connection_params _connection_params;

	_e_init_state	_init_state;

	CSocketStream _socket_stream;

	_e_work_loop_status _stream_end_status;

	std::mutex _wait_stream_mutex;
	std::unique_lock<std::mutex>* _lock_wait_stream;
	std::condition_variable _cv;

	// ���������� ��� ���������� ������ ������ ��������� ������
	void on_complete_stream_fn();

	// ����� ���������� ����� ������
	std::thread _listen_thread;

	// ������ ��������� ������
	_e_work_loop_status _work_loop_status;

	// ����� ������ ���������� �����
	void listen_method();

	e_socket_result init();

	void cleanup();

	// true ���� _socket_fn_result ����� valid_val
	bool check_socket_fn_result_and(const INT& valid_val);

	// true ���� _socket_fn_result �� ����� invalid_val
	bool check_socket_fn_result_not(const INT& invalid_val);

	CSingleServerSocket(const CSingleServerSocket& ob);

public:
	CSingleServerSocket(tools::lock_vector<data_wrappers::_tag_data_const>& received_data,
						std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSingleServerSocket();


	void Send(data_wrappers::_tag_data_const data);

	// ������� ���������� � ���������� �����������
	e_socket_result Start(const tag_connection_params& params);

	// �������� ������������� �����������
	e_socket_result Stop();
};

}
}