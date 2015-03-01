#pragma once
#include "stdafx.h"
#include "raw_data_warappers.h"
/*!
 * \file protocol_structures.h
 *
 * \author Victor Mulika
 * \date 01 ���� 2015
 *
 * ��������� ��� ������ ������� � ��������
 */

namespace server_exchange
{
// ��������� �������������� ������
enum class e_convert_result
{
	invalid_data,	// �������� ������
	success,		// �����
	empty_data		// ������ ���� ������
};

// ������ ������
const uint16_t begin_bytes = 0xf00d;

// ����� ������
const uint16_t end_bytes = 0x1013;

// ��� ������
enum class e_packet_type : byte
{
	unknown = 0,	// ����������
	id = 1			// ����� ������������� ���������
};

/******************************************************************************************
��������� ������������� ������
-------------------------------------------------------------------------------------
��������						|	�����	|	���		|	������������� ��������	|
-------------------------------------------------------------------------------------
������ ������					|	2		|	USHORT	|			0xF00D			|
��� ������						|	1		|	BYTE	|							|
����� ������					|	2		|	USHORT	|							|
������							|1...65536	| 	ARRAY	|							|
��������� ������				|	2		|	USHORT	|			0x1013			|
-------------------------------------------------------------------------------------
******************************************************************************************/
// ������������ �����, � ������� ���������� ������
struct tag_transport_packet 
{
	uint16_t								begin;		// ��������� ����� 0x0d � 0xf0
	e_packet_type							type;		// ��� ������
	uint16_t								length;		// ����� ������ (��� ����� ��������� ����)
	tools::data_wrappers::_tag_data_managed	data;		// �������
	uint16_t								end;		// ��������� ������

	tag_transport_packet()
		: begin (begin_bytes)
		, type (e_packet_type::unknown)
		, length (0)
		, end (end_bytes)
	{

	}
};


/******************************************************************************************
��������� ������ �������������
-------------------------------------------------------------------------------------
��������						|	�����	|	���		|	������������� ��������	|
-------------------------------------------------------------------------------------
�������� ������					|	20		|CHAR ARRAY	|							|
�������� ���������				|	20		|CHAR ARRAY	|							|
-------------------------------------------------------------------------------------
******************************************************************************************/

#pragma pack(push, 1)
// ����� ������������� ���������
struct tag_identification_packet
{
	char	group_name[20];		// �������� ������ ����������
	char	terminal_name[20];	// �������� ���������
};
#pragma pack(pop)


}



