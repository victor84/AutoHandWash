#include "stdafx.h"
#include "PacketParser.h"

using namespace server_exchange;

CPacketParser::CPacketParser()
{
}


CPacketParser::~CPacketParser()
{
}

e_convert_result CPacketParser::ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data, 
												   OUT std::vector<tag_transport_packet>& result_packets)
{
	if ((nullptr == data.p_data) || (0 == data.data_size))
		return e_convert_result::empty_data;

	if (data.data_size < 7)
		return e_convert_result::invalid_data;

	_raw_data.free_data();
	_raw_data.copy_data_inside(data.p_data, data.data_size);

	result_packets.clear();

	uint32_t offset = 0;

	e_convert_result result = e_convert_result::success;

	while (offset < _raw_data.data_size)
	{
		tag_transport_packet new_packet;

		if (e_convert_result::invalid_data == get_transport_packet(offset, new_packet))
		{
			result = e_convert_result::invalid_data;
			break;
		}
		result_packets.push_back(new_packet);
	}

	return result;
}

e_convert_result CPacketParser::get_transport_packet(IN OUT uint32_t& offset, OUT tag_transport_packet& result_packet)
{
	if ((_raw_data.data_size - offset) < 7)
		return e_convert_result::invalid_data;

	if (begin_byte != (uint16_t)_raw_data.p_data[offset])
		return e_convert_result::invalid_data;

	result_packet.begin = (uint16_t)_raw_data.p_data[offset];

	offset += 2;
	
	result_packet.type = static_cast<e_packet_type>(_raw_data.p_data[offset]);

	++offset;

	result_packet.length = (uint16_t)_raw_data.p_data[offset];

	offset += 2;

	result_packet.data.copy_data_inside(static_cast<void*>(&_raw_data.p_data[offset]), result_packet.length);

	offset += result_packet.length;

	result_packet.end = (uint16_t)_raw_data.p_data[offset];

	offset += 2;

	if (end_byte != result_packet.end)
		return e_convert_result::invalid_data;

	return e_convert_result::success;
}
