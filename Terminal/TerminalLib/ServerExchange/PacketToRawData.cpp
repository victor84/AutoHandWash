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
	uint32_t packet_size = 9 + packet.length;

	result_data.alloc_data(packet_size);

	byte* p_data = result_data.p_data;

	*(uint16_t*)p_data = packet.begin;
	p_data += 2;

	*p_data = static_cast<byte>(packet.type);
	++p_data;

	*(uint32_t*)p_data = packet.length;
	p_data += 4;

	if (0 != memcpy_s(p_data, packet_size - 9, packet.data.p_data, packet.data.data_size))
	{
		return e_convert_result::invalid_data;
	}

	p_data += packet.data.data_size;

	*(uint16_t*)p_data = packet.end;

	return e_convert_result::success;
}

e_convert_result CPacketToRawData::CreateLogRecordPacketRawData(IN const tag_log_record_packet& packet, 
																OUT tools::data_wrappers::_tag_data_managed& result_data)
{
	uint32_t packet_size = (11 + packet.text.data_size);

	result_data.alloc_data(packet_size);

	byte* p_data = result_data.p_data;

	*(uint64_t*)p_data = packet.date_time;
	p_data += sizeof(packet.date_time);

	*p_data = static_cast<byte>(packet.type);
	p_data += sizeof(packet.type);

	*(uint16_t*)p_data = packet.length;
	p_data += sizeof(packet.length);

	if (0 != memcpy_s(p_data, packet_size - 11, packet.text.p_data, packet.text.data_size))
	{
		return e_convert_result::invalid_data;
	}

	return e_convert_result::success;
}

