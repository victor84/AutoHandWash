#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"


/*!
 * \file PacketParser.h
 *
 * \author Victor Mulika
 * \date 01 Март 2015
 *
 * 
 */

namespace server_exchange
{

// результат парсинга
enum class e_parse_result
{
	invalid_data,	// неверные данные
	success,		// успех
	empty_data		// пустой блок данных
};

// Парсер пакетов обмена с сервером
class CPacketParser
{
	// сырые принятые данные
	tools::data_wrappers::_tag_data_managed _raw_data;

	// вычленение транспортного пакета из данных
	// начиная с указанного отступа
	// отступ обновляется
	e_parse_result get_transport_packet(IN OUT uint32_t& offset, OUT tag_transport_packet& result_packet);


public:
	CPacketParser();
	virtual ~CPacketParser();

	// парсинг данных транспортного уровня
	e_parse_result ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data,
										OUT std::vector<tag_transport_packet>& result_packets);


};

}