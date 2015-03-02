#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"


/*!
 * \file PacketParser.h
 *
 * \author Victor Mulika
 * \date 01 ћарт 2015
 *
 * 
 */

namespace server_exchange
{



// ѕарсер пакетов обмена с сервером
class CPacketParser
{
	// сырые прин€тые данные
	tools::data_wrappers::_tag_data_managed _raw_data;

	// вычленение транспортного пакета из данных
	// начина€ с указанного отступа
	// отступ обновл€етс€
	e_convert_result get_transport_packet(IN OUT uint32_t& offset, OUT tag_transport_packet& result_packet);


public:
	CPacketParser();
	virtual ~CPacketParser();

	// парсинг данных транспортного уровн€
	e_convert_result ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data,
										OUT std::vector<tag_transport_packet>& result_packets);


};

}