#pragma once
#include "stdafx.h"
/*!
 * \file device_protocol.h
 *
 * \author Victor Mulika
 * \date 03 ���� 2015
 *
 * ��������� ������ � �����������
 *
 */

namespace device_exchange
{

/******************************************************************************************
������ �������� ������ � EEPROM �����������
����� ������ = 256 ���� / 4 ����� �� ������ = 64 ������: 0x00 � 0x3F
0x00 � ���� ���� �����
0x01 � ������� ������
0x02 � ������� ���������������
0x03 � ������� ���������������
0x04 � ����� �������
0x05 � ��������� 1 ������ �������
0x06 � ����� ����� ������� ������� ����� ��������� �������
0x11 � 0x18 ��������� ������ ������� �������
0x21 � 0x28 ���� ������ ������ ������� �������
******************************************************************************************/

/******************************************************************************************
���������� ����� ������������� ��� ����� ������ ��� ��������� ������ � �������������� 
���������� ������� 0x02 �� ������ ������� ���������� �������� � �������. 
����� ����� ������� �� ���� 0x02 � 0x03 ��������������. �� ������ 0x00 � 0x01 
��� �� ����� ������.
PC ������ ��� �������� � ��� ���� ������
******************************************************************************************/

// ������� �� ����������
enum class e_command_from_device : byte
{
	// ������ �������
	empty = 0x00,

	// CMD=0x01 ������ ������, DATA0 = 0x01� 0x08
	button_press = 0x01,

	// CMD=0x02 ������ �� ��������������� ��� ���������������
	// DATA0 = �����, ������� ����� �������� � �������
	bill_acceptor = 0x02,

	// CMD=0x03 ������ ������ ��������. �������� ������ DATA0
	hopper_issue_coin = 0x03,

	// CMD=0x04 ������ ���������� �����
	discount_card_issued = 0x04,

	// CMD=0x0A �������� ������ �� EEPROM, DATA0=����� ������,
	// DATA1, DATA2, DATA3, DATA4 = 4 ����� ������ DATA1 �������, DATA4 �������
	data_from_eeprom = 0x0a,

	// CMD=0x0C ������ ������� ���� ������. DATA0=8���, ��������������� ������ ������.
	// 7� ��� = ������8, 0� ��� = ������1, 1 = ������ ������.
	buttons_state = 0x0c,

	// CMD=0x0E ������������� ���������� �������, DATA0=��� ������� ������� 
	// �������� �������������. ������������ ������ � ����� ������� �� PC
	command_confirmation = 0x0e,

	// CMD=0x0F ������, DATA0=��� ������:
	// 0x03 � ��� ����� � �������, DATA0 = ��� - �� ���������� �����
	// 0x04 � ���������� ����� �� ������
	// 0x0B � ������ ������ � EEPROM
	error = 0x0f

};

// ������� �� pc
enum class e_command_from_pc : byte
{
	// ������ �������
	empty = 0x00,

	// CMD=0x10 �������� ������. DATA0=0x01�0x08 � ����� �������
	open_valve = 0x10,

	// CMD=0x20 ��������� ������. DATA0=0x01�0x08 � ����� �������
	close_valve = 0x20,

	// CMD=0x30 ������ �������� ������. DATA0=���-�� �����. 
	// � ����� ����� ��������� ������� 0x03 � ���-��� ���������� �����.
	issue_coins = 0x30,

	// CMD=0x40 ������ ���������� �����.
	issue_discount_card = 0x40,

	// CMD=0xA0 ������� ������ �� EEPROM, DATA0=����� ������
	read_eeprom = 0xa0,

	// CMD=0xB0 �������� ������ �� EEPROM, DATA0=����� ������,
	// DATA1, DATA2, DATA3, DATA4 = 4 ����� ������ DATA1 �������, DATA4 �������
	write_eeprom = 0xb0,

	// CMD=0xC0 ������ ������� ���� ������.
	buttons_status
};

/******************************************************************************************
������ �������:
0x0A CMD DATA0 DATA1 DATA2 DATA3 DATA4 DATA5 CRC 0xA0, ���
0x0A � ������ ������
CMD � �������
DATA0 � ������0
DATA1 � ������1
DATA2 � ������2
DATA3 � ������3
DATA4 � ������4
DATA5 � ������5
CRC � ����������� ����� = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
0xA0 � ����� ������
******************************************************************************************/

// ��������� ����
const byte begin_byte = 0x0a;

// ����������� ����
const byte end_byte = 0xa0;

#pragma pack(push, 1)
// ������� �� ����������
struct tag_packet_from_device
{
	byte begin;						// 0x0A � ������ ������
	e_command_from_device command;	// CMD � �������
	byte data0;						// ������0
	byte data1;						// ������1
	byte data2;						// ������2
	byte data3;						// ������3
	byte data4;						// ������4
	byte data5;						// ������5
	byte crc;						// CRC � ����������� ����� = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
	byte end;						// ����� ������

	tag_packet_from_device()
	{
		clear();
	}

	void clear()
	{
		begin	= begin_byte;
		command	= e_command_from_device::empty;
		data0	= 0;
		data1	= 0;
		data2	= 0;
		data3	= 0;
		data4	= 0;
		data5	= 0;
		crc		= 0;
		end		= end_byte;
	}
};

// ������� �� pc
struct tag_packet_from_pc
{
	byte begin;					// 0x0A � ������ ������
	e_command_from_pc command;	// CMD � �������
	byte data0;					// ������0
	byte data1;					// ������1
	byte data2;					// ������2
	byte data3;					// ������3
	byte data4;					// ������4
	byte data5;					// ������5
	byte crc;					// CRC � ����������� ����� = CMD xor DATA0 xor DATA1 xor DATA2 xor DATA3 xor DATA4 xor DATA5
	byte end;					// ����� ������

	tag_packet_from_pc()
	{
		clear();
	}

	void clear()
	{
		begin = begin_byte;
		command = e_command_from_pc::empty;
		data0 = 0;
		data1 = 0;
		data2 = 0;
		data3 = 0;
		data4 = 0;
		data5 = 0;
		crc = 0;
		end = end_byte;
	}
};
#pragma pack(pop)

// ��������� �������������� ������
enum class e_convert_result
{
	invalid_data,	// �������� ������
	success,		// �����
	empty_data		// ������ ���� ������
};

}



