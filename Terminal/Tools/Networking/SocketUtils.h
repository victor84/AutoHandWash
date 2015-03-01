#pragma once
/*!
 * \file SocketUtils.h
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
// ��������� ���������� ������� �������
enum class e_socket_result
{
	success,			// �����
	error,				// ������
	was_connected,		// ���������� ���� �����������
	was_disconnected	// ���������� ���� ���������

};

// ��������� �����������
struct tag_connection_params
{
	std::string	address;				// �����
	std::string	port;					// ����
	int			reconnection_timeout;	// ������� ��������������� ��� ������� ����������
};
}
}
