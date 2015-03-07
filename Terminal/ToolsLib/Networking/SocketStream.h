#pragma once
#include "stdafx/stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "TraceError.h"
#include "SocketUtils.h"
#include <mutex>
#include <condition_variable>


/*!
 * \file SocketStream.h
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
// ����� ������, ������������ ������ � ������ � �����
// �������� � ��������� ������
class CSocketStream
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

	SOCKET	_client_socket;

	// ����� ��� ����� ������
	CHAR _received_buffer[0x10000];

	// ���������� �������� ����
	INT _received_bytes_count;

	// �������� ������
	tools::lock_vector<data_wrappers::_tag_data_const>& _received_data;

	// ������ ��� ��������
	tools::lock_vector<data_wrappers::_tag_data_const> _data_to_send;

	// ����� ��������� ������
	std::thread _this_thread;

	// ������ ��������� ������
	_e_work_loop_status _work_loop_status;

	// ���� ����� ������ ��������� ������
	_e_work_loop_status* _end_status;

	_e_init_state _start_state;

	std::function<void()> _on_complete_fn;

	void thread_method();

	// ������� ������
	void cleanup();

	// �������� ������ 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// ���� ������
	e_socket_result receive_data();

	// �������� ������
	e_socket_result send_data();

	CSocketStream(const CSocketStream&);

public:

	CSocketStream(tools::lock_vector<data_wrappers::_tag_data_const>& received_data);
	virtual ~CSocketStream();

	// ������
	e_socket_result Start(const SOCKET& socket_to_process,
						  _e_work_loop_status* end_status,
						  std::function<void()> on_complete_fn);

	// ���������
	e_socket_result Stop();

	void Send(data_wrappers::_tag_data_const data);

};
}
}



