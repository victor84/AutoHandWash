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

	// создание данных из пакетов данных
	template<typename _PacketType>
	e_convert_result CreateCommonPacketRawData(IN const _PacketType& packet, 
										 OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		result_data.copy_data_inside(&packet, sizeof(_PacketType));

		return e_convert_result::success;
	}

public:
	CPacketToRawData();
	virtual ~CPacketToRawData();

	// создание данных для отправки из пакета транспортного уровня
	e_convert_result CreateRawData(IN const tag_transport_packet& packet, 
								   OUT tools::data_wrappers::_tag_data_managed& result_data);

	// создание сырых данных идентификационного пакета
	e_convert_result CreateIdentificationPacketRawData(IN const tag_identification_packet& packet, 
													   OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета со счётчиками
	e_convert_result CreateCountersPacketRawData(IN const tag_counters_packet& packet, 
													   OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета с настройками
	e_convert_result CreateSettingsPacketRawData(IN const tag_settings_packet& packet, 
													   OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета с подтверждением
	e_convert_result CreateConfirmationPacketRawData(IN const tag_confirmation_packet& packet, 
													   OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета состояния терминала
	e_convert_result CreateTerminalStatePacketRawData(IN const tag_terminal_state_packet& packet,
													  OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета с записью лога
	e_convert_result CreateLogRecordPacketRawData(IN const tag_log_record_packet& packet,
												  OUT tools::data_wrappers::_tag_data_managed& result_data);

	// создание сырых данных пакета выданного приза
	e_convert_result CreateDistributePrizePacketRawData(IN const tag_distribute_prize_packet& packet,
														OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}

	// создание сырых данных пакета выдачи дисконтной карты
	e_convert_result CreateDistributeDiscountCardPacketRawData(IN const tag_distribute_discount_card_packet& packet,
														OUT tools::data_wrappers::_tag_data_managed& result_data)
	{
		return CreateCommonPacketRawData(packet, result_data);
	}
};
}




