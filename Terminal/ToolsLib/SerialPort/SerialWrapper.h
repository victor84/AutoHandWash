#pragma once
#include "SerialUtils.h"
#include "tools_structures.h"
#include "TraceError.h"
#include "raw_data_warappers.h"
#include "lock_deque.h"
#include "Serial.h"

namespace tools
{
namespace serial_port
{

class CSerialWrapper
{
	tools::logging::CTraceError* _tr_error;

	tag_connection_params _connection_params;

	// ��������� �������������
	tools::e_init_state _init_state;

	// �������� ������
	tools::lock_vector<data_wrappers::_tag_data_managed>& _received_data;

	// ������ ��� ��������
	std::shared_ptr<tools::lock_deque<data_wrappers::_tag_data_const>> _data_to_send;

	// ����� ��������� ������
	std::thread _this_thread;

	// ������ ��������� ������
	e_work_loop_status _work_loop_status;

	// ���� ���������� ������
	bool _thread_running;

	// COM ����
	CSerial _serial_port;

	// ����� ������ �����
	BYTE _read_buffer[1024];

	// ����� ��� ������ � ����
	BYTE _write_buffer[1024];

	// ���������� ��� ��������� ������
	std::function<void(tools::data_wrappers::_tag_data_managed)> _on_data_received;

	void thread_method();

	// ������������� �����
	e_serial_result init();

	// ������� �����
	void cleanup();

	// �������� ������ ������
	void clear_buffers();

	// ��������� ������ � �����
	e_serial_result receive_data();

	// �������� ������
	e_serial_result send_data();

public:
	CSerialWrapper(tools::lock_vector<data_wrappers::_tag_data_managed>& received_data,
				   std::function<void(tools::data_wrappers::_tag_data_managed)> on_data_received);
	virtual ~CSerialWrapper();

	// ������� ���� � ���������� �����������
	e_serial_result Start(const tag_connection_params& params);

	void Stop();

	// ���������� � ������� ������ �� ��������
	void PushBackToSend(data_wrappers::_tag_data_const data);

	// ���������� � ������ ������� �� ��������
	void PushFrontToSend(data_wrappers::_tag_data_const data);

};

}
}