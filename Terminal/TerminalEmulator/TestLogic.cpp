#include "stdafx.h"
#include "TestLogic.h"

using namespace server_exchange;

void CTestLogic::thread_method()
{
	if (false == send_identification_packet())
		_continue_work = false;

	while (true == _continue_work)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void CTestLogic::on_data_received(tools::data_wrappers::_tag_data_managed data)
{

}

bool CTestLogic::send_identification_packet()
{
	tag_identification_packet identification_packet;

	std::wstring group_name = _T("Хорошая группа");
	group_name._Copy_s(identification_packet.group_name, sizeof(identification_packet.group_name), group_name.size());

	std::wstring terminal_name = _T("Первый терминал");
	terminal_name._Copy_s(identification_packet.terminal_name, 
						  sizeof(identification_packet.terminal_name), 
						  terminal_name.size());

	tag_transport_packet transport_packet;
	transport_packet.length = sizeof(identification_packet);
	transport_packet.type = e_packet_type::id;
	_packet_to_raw_data.CreateIdentificationPacketRawData(identification_packet, transport_packet.data);

	tools::data_wrappers::_tag_data_managed raw_data;

	_packet_to_raw_data.CreateRawData(transport_packet, raw_data);

	_socket.Send(raw_data);

	return true;
}

CTestLogic::CTestLogic()
	: _socket(_received_data, std::bind(std::mem_fn(&CTestLogic::on_data_received), this, std::placeholders::_1))
{
	_tr_error = tools::logging::CTraceError::get_instance();
}

CTestLogic::~CTestLogic()
{
}

bool CTestLogic::Start(tools::networking::tag_connection_params connection_params)
{
	if (_continue_work == true)
		return true;

	_socket.OpenConnection(connection_params);

	_continue_work = true;
	_this_thread = std::thread(&CTestLogic::thread_method, this);
	
	return true;
}

void CTestLogic::Stop()
{
	if (false == _continue_work)
		return;

	_continue_work = false;

	if (true == _this_thread.joinable())
		_this_thread.join();
}
