#pragma once
#include "BaseStructuresConvertor.h"
#include "raw_data_warappers.h"
#include "device_protocol.h"
#include "data_from_pc.h"

/*!
 * \file PacketFromPcToLogicDataFromPcConverter.h
 *
 * \author Victor Mulika
 * \date 19 Март 2015
 *
 * 
 */

namespace logic_structures
{

class CPacketFromPcToLogicDataFromPcConverter : public CBaseStructuresConvertor <
		typename tools::data_wrappers::_tag_data_managed,
		typename device_exchange::tag_packet_from_pc,
		typename device_exchange::e_command_from_pc,
		typename std::shared_ptr<tag_base_data_from_pc >,
		CPacketFromPcToLogicDataFromPcConverter >
{
	virtual std::shared_ptr<tag_base_data_from_pc > handler_result_to_convert_result(tools::data_wrappers::_tag_data_managed& result) final;

	virtual std::shared_ptr<tag_base_data_from_pc > get_default_result() final;

	virtual void fill_handlers() final;

	// преобразователи pc -> логика
	tools::data_wrappers::_tag_data_managed convert_open_valve			(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_close_valve			(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_issue_coins			(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_issue_discount_card	(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_read_eeprom			(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_write_eeprom		(const device_exchange::tag_packet_from_pc& packet);
	tools::data_wrappers::_tag_data_managed convert_buttons_status		(const device_exchange::tag_packet_from_pc& packet);

public:
	CPacketFromPcToLogicDataFromPcConverter();
	virtual ~CPacketFromPcToLogicDataFromPcConverter();
};

}