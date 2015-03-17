#pragma once
#include "stdafx.h"
#include "device_protocol.h"

/*!
 * \file data_from_device.h
 *
 * \author Victor Mulika
 * \date 17 ���� 2015
 *
 * ��������� ������ �� ���������� ��� ������ � ������ ���������
 */


namespace logic_structures
{

// ������� ���������
struct tag_base_data_from_device
{
protected:

	const device_exchange::e_command_from_device command_id;	// id �������

	tag_base_data_from_device(device_exchange::e_command_from_device cid) : command_id(cid)
	{

	}
};

// ������ ������ (���ר� � 1)
struct tag_button_press : tag_base_data_from_device
{
	byte button_number;	// ����� ������� ������

	tag_button_press() 
		: tag_base_data_from_device(device_exchange::e_command_from_device::button_press)
		, button_number(0)
	{
		
	}
};

// ������ �� �������������� ��� ��������������
struct tag_bill_acceptor : tag_base_data_from_device
{
	uint16_t count;	// �����

	tag_bill_acceptor() : tag_base_data_from_device(device_exchange::e_command_from_device::bill_acceptor)
		, count(0)
	{

	}
};

// ������ ������ ��������
struct tag_hopper_issue_coin : tag_base_data_from_device
{
	uint16_t balance;	// �������� ������

	tag_hopper_issue_coin() : tag_base_data_from_device(device_exchange::e_command_from_device::hopper_issue_coin)
		, balance(0)
	{

	}
};

// ������ ���������� �����
struct tag_discount_card_issued : tag_base_data_from_device
{
	tag_discount_card_issued() : tag_base_data_from_device(device_exchange::e_command_from_device::discount_card_issued)
	{

	}
};

// ������ ���������� �����
struct tag_data_from_eeprom : tag_base_data_from_device
{
	byte cell_number;	// ����� ������
	uint32_t value;		// �������� ������

	tag_data_from_eeprom() : tag_base_data_from_device(device_exchange::e_command_from_device::data_from_eeprom)
		, cell_number(0)
		, value(0)
	{

	}
};

// �������� ������
struct tag_buttons_state : tag_base_data_from_device
{
	bool button1;	// ������ 1
	bool button2;	// ������ 2
	bool button3;	// ������ 3
	bool button4;	// ������ 4
	bool button5;	// ������ 5
	bool button6;	// ������ 6
	bool button7;	// ������ 7
	bool button8;	// ������ 8
	bool button9;	// ������ 9

	tag_buttons_state() : tag_base_data_from_device(device_exchange::e_command_from_device::buttons_state)
		,button1(false)
		,button2(false)
		,button3(false)
		,button4(false)
		,button5(false)
		,button6(false)
		,button7(false)
		,button8(false)
		,button9(false)
	{

	}
};

// ������������� �������
struct tag_command_confirmation : tag_base_data_from_device
{
	device_exchange::e_command_from_pc command;

	tag_command_confirmation() : tag_base_data_from_device(device_exchange::e_command_from_device::command_confirmation)
		, command(device_exchange::e_command_from_pc::empty)
	{

	}
};

// ��� ������ ����������
enum class e_device_error_code : byte
{
	error = 0x00,						// ����������� ������
	empty_hopper = 0x03,				// ��� ����� � �������, DATA0 = ��� - �� ���������� �����
	discount_card_not_issued = 0x04,	// ���������� ����� �� ������
	error_write_eeprom = 0x0B			// ������ ������ � EEPROM
};

// ������
struct tag_error : tag_base_data_from_device
{
	e_device_error_code	code;	// ��� ������

	tag_error() : tag_base_data_from_device(device_exchange::e_command_from_device::error)
		, code(e_device_error_code::error)
	{

	}
};



}