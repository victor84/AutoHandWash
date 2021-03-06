#pragma once
#include "stdafx/stdafx.h"
#include "lock_vector.h"
#include "raw_data_warappers.h"
#include "TraceError.h"
#include "SocketUtils.h"
#include <mutex>
#include <condition_variable>
#include "tools_structures.h"
#include "lock_deque.h"


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

	SOCKET	_socket;

	// ����� ��� ����� ������
	CHAR _received_buffer[0x10000];

	// ���������� �������� ����
	INT _received_bytes_count;

	// �������� ������
	tools::lock_vector<data_wrappers::_tag_data_managed>& _received_data;

	// ������ ��� ��������
	std::shared_ptr<tools::lock_deque<data_wrappers::_tag_data_const>> _data_to_send;

	// ����� ��������� ������
	std::thread _this_thread;

	// ������ ��������� ������
	e_work_loop_status _work_loop_status;

	// ���� ����� ������ ��������� ������
	e_work_loop_status* _end_status;

	tools::e_init_state _init_state;

	// ���������� �� ���������� ������
	std::function<void()> _on_complete_fn;

	// ���������� ��� ��������� ������
	std::function<void(tools::data_wrappers::_tag_data_managed)> _on_data_received;

	bool _thread_running;

	void thread_method();

	// ������� ������
	void cleanup();

	// �������� ������ ������
	void clear_buffers();

	// �������� ������ 
	_e_check_socket_result check_socket(const _e_check_socket_type& cst);

	// ���� ������
	e_socket_result receive_data();

	// �������� ������
	e_socket_result send_data();

	CSocketStream(const CSocketStream&);

public:

	CSocketStream(tools::lock_vector<data_wrappers::_tag_data_managed>& received_data,
				  std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSocketStream();

	// ������
	e_socket_result Start(const SOCKET& socket_to_process,
						  e_work_loop_status* end_status,
						  std::function<void()> on_complete_fn);

	// ���������
	e_socket_result Stop();

	// ���������� � ������� ������ �� ��������
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// ���������� � ������ ������� �� ��������
	void PushFrontToSend(data_wrappers::_tag_data_const data);

};
}
}



