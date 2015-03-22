#pragma once
#include "stdafx.h"
#include "protocol_structures.h"

/*!
 * \file server_structures.h
 *
 * \author Victor Mulika
 * \date 21 ���� 2015
 *
 * ��������� ������ ������ � �������� ��� ������ � ������
 */

namespace logic_structures
{

// ������� ����� ��������� ������ ��� ������ � ������
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

// ��������� ������ ��������� ������ ��� ������ � ������
template<typename _PacketType, server_exchange::e_packet_type _Id>
struct tag_server_logic_packet : tag_base_server_logic_struct
{
	// ����� � �������
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