#include "stdafx.h"
#include "PacketToRawData.h"

using namespace server_exchange;

CPacketToRawData::CPacketToRawData()
{
}


CPacketToRawData::~CPacketToRawData()
{
}

e_convert_result CPacketToRawData::CreateRawData(IN const tag_transport_packet& packet, 
												 OUT tools::data_wrappers::_tag_data_managed& result_data)
{
	uint16_t packet_size = 7 + packet.length;

	result_data.alloc_data(packet_size);

	byte* p_data = result_data.p_data;

	*(uint16_t*)p_data = packet.begin;
	p_data += 2;

	*p_data = static_cast<byte>(packet.type);
	++p_data;

	*(uint16_t*)p_data = packet.length;
	p_data += 2;

	if (0 != memcpy_s(p_data, packet_size - 7, packet.data.p_data, packet.data.data_size))
	{
		return e_convert_result::invalid_data;
	}

	p_data += packet.data.data_size;

	*(uint16_t*)p_data = end_bytes;

	return e_convert_result::success;
}

