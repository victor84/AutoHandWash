#pragma once
#include "TraceError.h"
#include "device_protocol.h"
/*!
 * \file DeveiceMessageShower.h
 *
 * \author Victor Mulika
 * \date 09 ���� 2015
 *
 * 
 */


/*
CDeveiceMessageShower
���������� ���������� � ���������
*/
class CDeveiceMessageDescription
{
	typedef std::function<bool(const device_exchange::tag_packet_from_device& message)>	_Descriptor_Fn_Type;
	typedef std::pair<device_exchange::e_command_from_device, _Descriptor_Fn_Type>		_Storage_Elem_Type;


	tools::logging::CTraceError* _tr_error;

	std::wstringstream _str_str;

	// ��������� ��� �������� ������� � �������-���������� ���������
	std::map<device_exchange::e_command_from_device, _Descriptor_Fn_Type> _descriptors_storage;
	
	void fill_descriptors_storage();

	// ������� ����� ������
	bool describe_raw(const device_exchange::tag_packet_from_device& message);

	// ��������� e_command_from_device::button_press
	bool button_press(const device_exchange::tag_packet_from_device& message);



public:
	CDeveiceMessageDescription();
	virtual ~CDeveiceMessageDescription();

	// ������� ���������� � ���������
	std::wstring describe_message(const device_exchange::tag_packet_from_device& message);


};

