#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"


/*!
 * \file PacketParser.h
 *
 * \author Victor Mulika
 * \date 01 ���� 2015
 *
 * 
 */

namespace server_exchange
{

// ��������� ��������
enum class e_parse_result
{
	invalid_data,	// �������� ������
	success,		// �����
	empty_data		// ������ ���� ������
};

// ������ ������� ������ � ��������
class CPacketParser
{
	// ����� �������� ������
	tools::data_wrappers::_tag_data_managed _raw_data;

	// ���������� ������������� ������ �� ������
	// ������� � ���������� �������
	// ������ �����������
	e_parse_result get_transport_packet(IN OUT uint32_t& offset, OUT tag_transport_packet& result_packet);


public:
	CPacketParser();
	virtual ~CPacketParser();

	// ������� ������ ������������� ������
	e_parse_result ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data,
										OUT std::vector<tag_transport_packet>& result_packets);


};

}