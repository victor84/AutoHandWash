#pragma once
#include "BaseStructuresConvertor.h"
#include "device_protocol.h"
#include "data_from_pc.h"

namespace logic_structures
{
/*
CLogicDataFromPcToPacketFromPcConverter
logic packet from pc -> hardware pc packet
*/
class CLogicDataFromPcToPacketFromPcConverter : public CBaseStructuresConvertor <
														typename device_exchange::tag_packet_from_pc,
														typename std::shared_ptr<tag_base_data_from_pc>,
														typename device_exchange::e_command_from_pc,
														typename device_exchange::tag_packet_from_pc,
														 CLogicDataFromPcToPacketFromPcConverter>
{
	virtual void fill_handlers() final;

	virtual device_exchange::tag_packet_from_pc handler_result_to_convert_result(device_exchange::tag_packet_from_pc& result) final;

	virtual device_exchange::tag_packet_from_pc get_default_result() final;

	// преобразователи pc -> логика
	device_exchange::tag_packet_from_pc convert_open_valve			(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_close_valve			(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_issue_coins			(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_issue_discount_card	(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_read_eeprom			(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_write_eeprom		(const std::shared_ptr<tag_base_data_from_pc>& packet);
	device_exchange::tag_packet_from_pc convert_buttons_status		(const std::shared_ptr<tag_base_data_from_pc>& packet);



public:
	CLogicDataFromPcToPacketFromPcConverter();
	virtual ~CLogicDataFromPcToPacketFromPcConverter();
};

}