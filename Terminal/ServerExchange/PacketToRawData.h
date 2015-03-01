#pragma once
#include "raw_data_warappers.h"
#include "protocol_structures.h"

/*!
 * \file PacketCreator.h
 *
 * \author Victor Mulika
 * \date 01 Март 2015
 *
 * 
 */

namespace server_exchange
{
// создаёт сырые данные из пакета
class CPacketToRawData
{
public:
	CPacketToRawData();
	virtual ~CPacketToRawData();

	// создание данных для отправки из пакета транспортного уровня
	e_convert_result CreateRawData(IN const tag_transport_packet& packet,
								OUT tools::data_wrappers::_tag_data_managed& result_data);



};
}




