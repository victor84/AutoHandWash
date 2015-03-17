#pragma once
#include "stdafx.h"
#include "device_protocol.h"

/*!
 * \file data_from_pc.h
 *
 * \author Victor Mulika
 * \date 17 ���� 2015
 *
 * ��������� ������ �� ���������� ��� ������ � ������ ���������
 */


namespace logic_structures
{
	// ������� ���������
	struct tag_base_data_from_pc
	{
	protected:

		const device_exchange::e_command_from_pc command_id;	// id �������

		tag_base_data_from_pc(device_exchange::e_command_from_pc cid) : command_id(cid)
		{

		}
	};

	// ������ �������
	struct tag_pc_empty : tag_base_data_from_pc
	{
		tag_pc_empty() : tag_base_data_from_pc(e_command_from_pc::empty)
		{

		}
	};

	// ������� ������ (���ר� � 1)
	struct tag_open_valve : tag_base_data_from_pc
	{
		byte number;	// ����� ������������ �������

		tag_open_valve() : tag_base_data_from_pc(device_exchange::e_command_from_pc::open_valve)
			, number(0)
		{

		}
	};

	// ������� ������ (���ר� � 1)
	struct tag_close_valve : tag_base_data_from_pc
	{
		byte number;	// ����� ������������ �������

		tag_close_valve() : tag_base_data_from_pc(device_exchange::e_command_from_pc::close_valve)
			, number(0)
		{

		}
	};

	// ������ ������
	struct tag_issue_coins : tag_base_data_from_pc
	{
		byte number;	// ���������� ���������� �����

		tag_issue_coins() : tag_base_data_from_pc(device_exchange::e_command_from_pc::issue_coins)
			, number(0)
		{

		}
	};

	// ������ ���������� �����
	struct tag_issue_discount_card : tag_base_data_from_pc
	{
		tag_issue_discount_card() : tag_base_data_from_pc(device_exchange::e_command_from_pc::issue_discount_card)
		{

		}
	};

	// ��������� ������ eeprom
	struct tag_read_eeprom : tag_base_data_from_pc
	{
		byte cell_number;	// ����� ������

		tag_read_eeprom() : tag_base_data_from_pc(device_exchange::e_command_from_pc::read_eeprom)
			, cell_number(0)
		{

		}
	};

	// �������� ������ eeprom
	struct tag_write_eeprom : tag_base_data_from_pc
	{
		byte		cell_number;	// ����� ������
		uint32_t	value;			// ��������

		tag_write_eeprom() : tag_base_data_from_pc(device_exchange::e_command_from_pc::write_eeprom)
			, cell_number(0)
			, value(0)
		{

		}
	};

	// ��������� ������ ���� ������
	struct tag_buttons_status : tag_base_data_from_pc
	{
		tag_buttons_status() : tag_base_data_from_pc(device_exchange::e_command_from_pc::buttons_status)
		{

		}
	};

}