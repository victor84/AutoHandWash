#pragma once
#include "stdafx.h"
#include "protocol_structures.h"

/*!
 * \file server_structures.h
 *
 * \author Victor Mulika
 * \date 21 Март 2015
 *
 * Структуры обмена данных с сервером для работы в логике
 */

namespace logic_structures
{

// базовый пакет серверных данных для работы в логике
struct tag_base_server_logic_struct 
{
	const server_exchange::e_packet_type type;

	tag_base_server_logic_struct(const server_exchange::e_packet_type& type)
		: type(type)
	{

	}

	virtual ~tag_base_server_logic_struct()
	{

	}
};

// структура пакета серверных данных для работы в логике
template<typename _PacketType, server_exchange::e_packet_type _Id>
struct tag_server_logic_packet : tag_base_server_logic_struct
{
	// пакет с данными
	_PacketType packet;

	tag_server_logic_packet(const _PacketType& packet) 
		: tag_base_server_logic_struct(_Id)
		, packet(packet)
	{

	}

	virtual ~tag_server_logic_packet()
	{

	}

	operator tools::data_wrappers::_tag_data_managed()
	{
		tools::data_wrappers::_tag_data_managed result;
		result.copy_data_inside(this, sizeof(*this));
		return result;
	}
};


}