#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"
#include "TraceError.h"


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

// Парсер пакетов обмена с сервером
class CServerPacketParser
{
	// вычленение транспортного пакета из данных
	// начиная с указанного отступа
	// отступ обновляется
	e_convert_result get_transport_packet(IN OUT uint32_t& offset, 
										  IN const tools::data_wrappers::_tag_data_const& data,
										  OUT tag_transport_packet& result_packet);

	// парсинг данных пакета с прямым приведением данных
	template<typename _PacketType>
	e_convert_result ParseCommonPacket(IN const tag_transport_packet& data,
									   OUT _PacketType& result_packet)
	{
		if (sizeof(_PacketType) != data.length)
			return e_convert_result::invalid_data;

		result_packet = *(reinterpret_cast<_PacketType*>(data.data.p_data));

		return e_convert_result::success;
	}

public:
	CServerPacketParser();
	virtual ~CServerPacketParser();

	// парсинг данных транспортного уровня
	e_convert_result ParseTransportPacket(IN const tools::data_wrappers::_tag_data_const& data,
										OUT std::vector<tag_transport_packet>& result_packets);

	// парсинг данных пакета со счётчиками
	e_convert_result ParseCountersPacket(IN const tag_transport_packet& data,
										 OUT tag_counters_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

	// парсинг данных пакета с настройками
	e_convert_result ParseSettingsPacket(IN const tag_transport_packet& data,
										 OUT tag_settings_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

	// парсинг данных пакета с подтверджением
	e_convert_result ParseConfirmationPacket(IN const tag_transport_packet& data, 
											 OUT tag_confirmation_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

	// парсинг пакета пополнения баланса
	e_convert_result ParseRefillCachePacket(IN const tag_transport_packet& data,
											OUT tag_refill_cache_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

	// парсинг пакета приза
	e_convert_result ParsePrizePacket(IN const tag_transport_packet& data,
											OUT tag_prize_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

	// парсинг пакета состояния
	e_convert_result ParseTerminalStatePacket(IN const tag_transport_packet& data,
											  OUT tag_terminal_state_packet& result_packet)
	{
		return ParseCommonPacket(data, result_packet);
	}

};

}