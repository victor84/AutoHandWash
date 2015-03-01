#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"

/*!
 * \file PacketCreator.h
 *
 * \author Victor Mulika
 * \date 01 ���� 2015
 *
 * 
 */

namespace server_exchange
{
// ������ ����� ������ �� ������
class CPacketToRawData
{
public:
	CPacketToRawData();
	virtual ~CPacketToRawData();

	// �������� ������ ��� �������� �� ������ ������������� ������
	e_convert_result CreateRawData(IN const tag_transport_packet& packet,
								OUT tools::data_wrappers::_tag_data_managed& result_data);



};
}




