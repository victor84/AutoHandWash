#include "stdafx.h"
#include "ServerPacketParser.h"

using namespace server_exchange;

CServerPacketParser::CServerPacketParser()
{
}

CServerPacketParser::~CServerPacketParser()
{
}

e_convert_result CServerPacketParser::ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data, 
												   OUT std::vector<tag_transport_packet>& result_packets)
{
	if ((nullptr == data.p_data) || (0 == data.data_size))
		return e_convert_result::empty_data;

	if (data.data_size < 7)
		return e_convert_result::invalid_data;

	result_packets.clear();

	uint32_t offset = 0;

	e_convert_result result = e_convert_result::success;

	while (offset < data.data_size)
	{
		tag_transport_packet new_packet;

		if (e_convert_result::invalid_data == get_transport_packet(offset, data, new_packet))
		{
			result = e_convert_result::invalid_data;
			break;
		}
		result_packets.push_back(new_packet);
	}

	return result;
}

e_convert_result CServerPacketParser::get_transport_packet(IN OUT uint32_t& offset, 
														   IN const tools::data_wrappers::_tag_data_const& data,
														   OUT tag_transport_packet& result_packet)
{
	if ((data.data_size - offset) < 7)
		return e_convert_result::invalid_data;

	if (begin_bytes != *((uint16_t*)&data.p_data[offset]))
		return e_convert_result::invalid_data;

	result_packet.begin = *((uint16_t*)&data.p_data[offset]);
	offset += 2;
	
	result_packet.type = static_cast<e_packet_type>(data.p_data[offset++]);

	result_packet.length = *((uint16_t*)&data.p_data[offset]);
	offset += 2;

	result_packet.data.copy_data_inside(static_cast<const void*>(&data.p_data[offset]), result_packet.length);
	offset += result_packet.length;

	result_packet.end = *((uint16_t*)&data.p_data[offset]);
	offset += 2;

	if (end_bytes != result_packet.end)
		return e_convert_result::invalid_data;

	return e_convert_result::success;
}



