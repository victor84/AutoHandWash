#pragma once

namespace tools
{
namespace serial_port
{

// ��������� ���������� ������� �����
enum class e_serial_result
{
	success,			// �����
	error,				// ������
	was_connected,		// ���������� ���� ����� �����������
	was_disconnected,	// ���������� ���� ����� ���������
	no_data				// ��� ������
};

// ��������� ����������� � COM-�����
struct tag_connection_params
{
	std::wstring	port_name;		// �������� �����
	uint32_t		baud_rate;		// ��������
	uint32_t		bits_per_byte;	// ��� � �����
	uint32_t		stop_bits;		// ���������� ����-�����

	tag_connection_params()
	{
		port_name = _T("");
		baud_rate = 9600;
		bits_per_byte = 8;
		stop_bits = 1;
	}
};
}
}