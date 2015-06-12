#include "stdafx.h"
#include "Logic.h"
#include "AdvertisingIdleState.h"
#include "RefillCacheState.h"
#include "ExecutingServiceState.h"
#include "data_from_device.h"
#include "SettingsWorkState.h"
#include "FreeEdleState.h"
#include "PaidIdleState.h"
#include "DistributionOfPrizeState.h"

void logic::CLogic::fill_states()
{
	_states.clear();

	_states.insert(std::make_pair(e_state::advertising_idle, std::make_shared<CAdvertisingIdleState>(*(dynamic_cast<CLogicAbstract*>(this)), _correspond_settings, _common_settings)));
	_states.insert(std::make_pair(e_state::refill_cache, std::make_shared<CRefillCacheState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::executing_service, std::make_shared<CExecutingServiceState>(*(dynamic_cast<CLogicAbstract*>(this)), _correspond_settings)));
	_states.insert(std::make_pair(e_state::settings_work, std::make_shared<CSettingsWorkState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::free_idle, std::make_shared<CFreeEdleState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::paid_idle, std::make_shared<CPaidIdleState>(*(dynamic_cast<CLogicAbstract*>(this)))));
	_states.insert(std::make_pair(e_state::distribution_of_prize, std::make_shared<CDistributionOfPrizeState>(*(dynamic_cast<CLogicAbstract*>(this)))));
}

void logic::CLogic::thread_fn()
{
	set_state(e_state::settings_work);
	_current_state = get_state(e_state::settings_work);
	CSettingsWorkState* sws = dynamic_cast<CSettingsWorkState*>(_current_state.get());
	sws->read_settings();

	if (0 == _common_settings.GetState())
		_device_not_available = true;

	while (tools::e_work_loop_status::ok == _work_loop_status)
	{
		process_messages_from_device();

		process_messages_from_server();

		if (true == _device_not_available)
		{
			change_terminal_state(e_terminal_state::broken, true);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

tools::e_init_state logic::CLogic::init()
{
	if (tools::e_init_state::was_init == _init_state)
		return _init_state;

	if (false == _common_settings.ReadSettings())
		return tools::e_init_state::not_init;

	if (false == _correspond_settings.ReadSettings())
		return tools::e_init_state::not_init;

	if (_on_service_info_readed)
		send_services_info();

	_device_interact.Start();

	_work_loop_status = tools::e_work_loop_status::ok;
	_this_thread = std::thread(&CLogic::thread_fn, this);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	_server_interact.Start();

	_init_state = tools::e_init_state::was_init;
	return _init_state;
}

std::vector<logic::tag_service_info> logic::CLogic::get_services_info()
{
	std::vector<tag_service_info> result;

	for (byte i = 0; i < 9; ++i)
	{
		e_service_name sn = static_cast<e_service_name>(i);

		if (e_service_name::stop != sn)
		{
			tag_service_info si;

			si.button_number = _correspond_settings.GetButtonNumber(sn);
			si.service_name = _correspond_settings.GetServiceName(sn);
			si.cost = _correspond_settings.GetServiceCost(sn);
			si.id = sn;

			result.push_back(si);
		}
	}

	return result;
}

void logic::CLogic::send_services_info()
{
	std::vector<tag_service_info> result = get_services_info();

	if (false == result.empty())
	{
		_on_service_info_readed(result);
	}
}

void logic::CLogic::on_connected_to_server()
{
	send_settings_packet();
	send_identification_packet();
}

void logic::CLogic::on_disconnected_from_derver()
{

}

void logic::CLogic::send_identification_packet()
{
	server_exchange::tag_identification_packet identification_packet;

	std::wstring terminal_name = _common_settings.GetTerminalName();
	std::wstring terminal_group = _common_settings.GetTerminalGroup();

	terminal_name._Copy_s(identification_packet.terminal_name, sizeof(identification_packet.terminal_name), terminal_name.size());
	terminal_group._Copy_s(identification_packet.group_name, sizeof(identification_packet.group_name), terminal_group.size());

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet = 
		create_server_packet<server_exchange::tag_identification_packet, server_exchange::e_packet_type::id>(identification_packet);

	_server_interact.PushFrontToSend(packet);
}

void logic::CLogic::send_settings_packet()
{
	server_exchange::tag_settings_packet settings_packet;
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);

	logic::tag_device_settings device_settings = sws->get_settings();

	settings_packet.state = static_cast<logic::e_terminal_state>(device_settings.state);

	settings_packet.bill_acceptor_impulse = static_cast<byte>(device_settings.bill_acceptor_impulse);
	settings_packet.coin_acceptor_impulse = static_cast<byte>(device_settings.coin_acceptor_impulse);
	settings_packet.free_idle_time = static_cast<byte>(device_settings.free_idle_time);
	settings_packet.idle_time_cost = static_cast<uint16_t>(device_settings.idle_time_cost);
	settings_packet.pause_before_advertising = static_cast<byte>(device_settings.pause_before_advertising);
	settings_packet.frost_protection_value = device_settings.frost_protection_value;
	settings_packet.discount_card_condition = device_settings.discount_card_condition;

	settings_packet.pressurized_water = device_settings.cost_pressurized_water;
	settings_packet.water_without_pressure = device_settings.cost_water_without_pressure;
	settings_packet.foam = device_settings.cost_foam;
	settings_packet.wax = device_settings.cost_wax;
	settings_packet.vacuum_cleaner = device_settings.cost_vacuum_cleaner;
	settings_packet.air = device_settings.cost_air;
	settings_packet.osmosis = device_settings.cost_osmosis;

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet =
		create_server_packet<server_exchange::tag_settings_packet, server_exchange::e_packet_type::settings>(settings_packet);

	_server_interact.PushFrontToSend(packet);
}

void logic::CLogic::send_counters_packet()
{
	server_exchange::tag_counters_packet counters_packet;
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);

	logic::tag_device_settings device_settings = sws->get_settings();

	counters_packet.state = static_cast<logic::e_terminal_state>(device_settings.state);

	counters_packet.date_time = std::time(0);
	counters_packet.total_cache = device_settings.total_cache;
	counters_packet.current_cache = device_settings.current_cache;
	counters_packet.pressurized_water = device_settings.time_pressurized_water;
	counters_packet.water_without_pressure = device_settings.time_water_without_pressure;
	counters_packet.foam = device_settings.time_foam;
	counters_packet.wax = device_settings.time_wax;
	counters_packet.vacuum_cleaner = device_settings.time_vacuum_cleaner;
	counters_packet.air = device_settings.time_air;
	counters_packet.osmosis = device_settings.time_osmosis;

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet =
		create_server_packet<server_exchange::tag_counters_packet, server_exchange::e_packet_type::counters>(counters_packet);

	_server_interact.PushBackToSend(packet);
}

void logic::CLogic::send_confirmation_packet(server_exchange::e_packet_type packet_type, server_exchange::e_processing_result result)
{
	server_exchange::tag_confirmation_packet confirmation_packet;

	confirmation_packet.result = result;
	confirmation_packet.packet_type = packet_type;

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet =
		create_server_packet<server_exchange::tag_confirmation_packet, server_exchange::e_packet_type::confirmation>(confirmation_packet);

	_server_interact.PushBackToSend(packet);
}

void logic::CLogic::send_terminal_state_packet(const e_terminal_state& state)
{
	server_exchange::tag_terminal_state_packet terminal_state_packet;

	terminal_state_packet.state = state;

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet =
		create_server_packet<server_exchange::tag_terminal_state_packet, server_exchange::e_packet_type::terminal_state>(terminal_state_packet);

	_server_interact.PushBackToSend(packet);
}

void logic::CLogic::change_terminal_state(const e_terminal_state& state, bool send_to_server /*= false*/, bool write_to_file /*= false*/)
{
	if (true == _terminal_state_changed_sended)
		return;

	if (_common_settings.GetState() == static_cast<uint32_t>(state))
		return;

	if (_on_terminal_state_changed)
		_on_terminal_state_changed(state);

	if (true == send_to_server)
	{
		send_terminal_state_packet(state);
		send_counters_packet();
	}

	if (true == write_to_file)
	{
		_common_settings.SetState(static_cast<uint32_t>(state));
		_common_settings.ReadSettings();
	}

	_terminal_state_changed_sended = true;
}

void logic::CLogic::on_device_not_available()
{
	_device_not_available = true;
	_terminal_state_changed_sended = false;
}

logic::CLogic::CLogic()
	: _device_interact(_common_settings, 
						_packets_from_device, 
						_packets_to_device, 
						std::bind(std::mem_fn(&CLogic::on_device_not_available), this))
	, _server_interact(_common_settings, _packets_from_server, _packets_to_server, 
						std::bind(std::mem_fn(&CLogic::on_connected_to_server), this), 
						std::bind(std::mem_fn(&CLogic::on_disconnected_from_derver), this))
	, _need_update_device_settings(false)
	, _need_distribute_prize(false)
	, _device_not_available(false)
	, _terminal_state_changed_sended(false)
{
	_tr_error = tools::logging::CTraceError::get_instance();
	fill_states();
}

logic::CLogic::~CLogic()
{
}

void logic::CLogic::issue_discount_card()
{
	std::shared_ptr<logic_structures::tag_issue_discount_card> idc_message = std::make_shared<logic_structures::tag_issue_discount_card>();

	_packets_to_device.push_back(idc_message);
}

void logic::CLogic::send_log_to_server(server_exchange::e_log_record_type type, const std::wstring& text)
{
	server_exchange::tag_log_record_packet log_packet;

	log_packet.date_time = std::time(0);
	log_packet.type = type;
	log_packet.text.copy_data_inside(text.c_str(), text.size() * 2);
	log_packet.length = static_cast<uint16_t>(text.size() * 2);

	std::shared_ptr<logic_structures::tag_base_server_logic_struct> packet =
		create_server_packet<server_exchange::tag_log_record_packet, server_exchange::e_packet_type::log>(log_packet);

	_server_interact.PushBackToSend(packet);
}

void logic::CLogic::SetOnShowCountersFn(std::function<void(std::vector<logic::tag_service_counter>) > fn)
{
	_on_show_counters = fn;
}

void logic::CLogic::show_counters()
{
	if (!_on_show_counters)
		return;

	std::vector<logic::tag_service_counter> counters;

	std::vector<tag_service_info> services = get_services_info();

	if (true == services.empty())
		return;

	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	logic::tag_device_settings settings = sws->get_settings();

	for (tag_service_info si : services)
	{
		tag_service_counter sc;

		sc.service = si;

		switch (si.id)
		{
			case e_service_name::air:						sc.counter = static_cast<uint16_t>(settings.time_air);						break;
			case e_service_name::foam:						sc.counter = static_cast<uint16_t>(settings.time_foam);						break;
			case e_service_name::osmosis:					sc.counter = static_cast<uint16_t>(settings.time_osmosis);					break;
			case e_service_name::pressurized_water:			sc.counter = static_cast<uint16_t>(settings.time_pressurized_water);		break;
			case e_service_name::vacuum_cleaner:			sc.counter = static_cast<uint16_t>(settings.time_vacuum_cleaner);			break;
			case e_service_name::water_without_pressure:	sc.counter = static_cast<uint16_t>(settings.time_water_without_pressure);	break;
			case e_service_name::wax:						sc.counter = static_cast<uint16_t>(settings.time_wax);						break;
			default: continue;
		}

		counters.push_back(sc);
	}

	_on_show_counters(counters);
}

void logic::CLogic::SetOnTerminalStateChangedFn(std::function<void(logic::e_terminal_state) > fn)
{
	_on_terminal_state_changed = fn;
}

void logic::CLogic::show_advertising()
{
	if (_on_show_advertising)
		_on_show_advertising();
}

void logic::CLogic::read_buttons_status()
{
	std::shared_ptr<logic_structures::tag_buttons_status> bs_message = std::make_shared<logic_structures::tag_buttons_status>();

	_packets_to_device.push_back(bs_message);
}

void logic::CLogic::SetOnShowAdvertisingFn(std::function<void(void) > fn)
{
	_on_show_advertising = fn;
}

void logic::CLogic::SetOnEmptyHopperFn(std::function<void(void) > fn)
{
	_on_empty_hopper = fn;
}

void logic::CLogic::on_empty_hopper()
{
	if (_on_empty_hopper)
		_on_empty_hopper();

	send_log_to_server(server_exchange::e_log_record_type::warning, _T("Ошибка выдачи монет."));
}

void logic::CLogic::SetOnDistributionPrizeFn(std::function<void(int16_t, byte) > fn)
{
	_on_distribute_prize = fn;
}

void logic::CLogic::coin_issued(byte rest_of_coins)
{
	if (_on_distribute_prize)
		_on_distribute_prize(_prize_packet.prize_size, rest_of_coins);
}

void logic::CLogic::send_issue_coins_packet_to_device(byte count)
{
	std::shared_ptr<logic_structures::tag_issue_coins> issue_coins_packet = std::make_shared<logic_structures::tag_issue_coins>();

	issue_coins_packet->number = count;

	_packets_to_device.push_back(issue_coins_packet);
}

void logic::CLogic::on_counters_changed()
{
	send_counters_packet();
}

void logic::CLogic::on_settings_readed()
{
	if (false == _server_interact.IsStarted())
		_server_interact.Start();
}

void logic::CLogic::SetOnServiceInfoReadedFn(std::function<void(std::vector<tag_service_info>) > fn)
{
	_on_service_info_readed = fn;
}

void logic::CLogic::SetOnCacheRefilledFn(std::function<void(uint16_t) > fn)
{
	CRefillCacheState* rcs = get_implemented_state<CRefillCacheState>(e_state::refill_cache);

	rcs->set_on_cache_refilled_fn(fn);
}

bool logic::CLogic::Start()
{
	if (tools::e_init_state::was_init == _init_state)
		return true;

	return tools::e_init_state::was_init == init();
}

void logic::CLogic::Stop()
{
	if (tools::e_init_state::not_init == _init_state)
		return;

	_device_interact.Stop();
	_server_interact.Stop();

	_work_loop_status = tools::e_work_loop_status::stop;

	if (_this_thread.joinable())
		_this_thread.join();

	_init_state = tools::e_init_state::not_init;

	std::this_thread::sleep_for(std::chrono::seconds(3));
}

void logic::CLogic::process_messages_from_server()
{
	std::vector<std::shared_ptr<logic_structures::tag_base_server_logic_struct>> messages =
		_packets_from_server.get_with_cleanup();

	for (std::shared_ptr<logic_structures::tag_base_server_logic_struct> message : messages)
	{
		server_exchange::e_processing_result processing_result = server_exchange::e_processing_result::failed;

		if (true == process_server_message(message, processing_result))
			send_confirmation_packet(message->type, processing_result);
	}
}

bool logic::CLogic::process_server_message(std::shared_ptr<logic_structures::tag_base_server_logic_struct> message, server_exchange::e_processing_result& processing_result)
{
	if (server_exchange::e_packet_type::settings == message->type)
	{
		_settings_from_server = get_server_message<server_exchange::tag_settings_packet, server_exchange::e_packet_type::settings>(message);
		_need_update_device_settings = true;
		processing_result = server_exchange::e_processing_result::success;

		if (e_state::advertising_idle == _current_state->get_this_state())
		{
			update_device_settings_from_server();
		}

		return true;
	}
	else if (server_exchange::e_packet_type::confirmation == message->type)
	{
		processing_result = server_exchange::e_processing_result::success;
		return false;
	}
	else if (server_exchange::e_packet_type::refill_cache == message->type)
	{
		CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
		logic::tag_device_settings device_settings = sws->get_settings();

		server_exchange::tag_refill_cache_packet refill_cache_packet = get_server_message<server_exchange::tag_refill_cache_packet, server_exchange::e_packet_type::refill_cache>(message);

		uint16_t impulses_count = static_cast<uint16_t>(static_cast<uint32_t>(refill_cache_packet.cache) / device_settings.bill_acceptor_impulse);
		
		while (0 < impulses_count--)
		{
			_current_state->refilled_cache();
		}

		processing_result = server_exchange::e_processing_result::success;
		return true;
	}
	else if (server_exchange::e_packet_type::prize == message->type)
	{
		_prize_packet = get_server_message<server_exchange::tag_prize_packet, server_exchange::e_packet_type::prize>(message);

		_need_distribute_prize = true;
		if (e_state::advertising_idle == _current_state->get_this_state())
		{
			distribute_prize();
		}

		processing_result = server_exchange::e_processing_result::success;
		return true;
	}
	else if (server_exchange::e_packet_type::terminal_state == message->type)
	{
		server_exchange::tag_terminal_state_packet termianl_state_packet = get_server_message<server_exchange::tag_terminal_state_packet, server_exchange::e_packet_type::terminal_state>(message);

		_terminal_state_changed_sended = false;
		change_terminal_state(termianl_state_packet.state, false, true);
		send_counters_packet();

		processing_result = server_exchange::e_processing_result::success;
		return true;
	}
	else
	{
		processing_result = server_exchange::e_processing_result::success;
		return true;
	}
}

void logic::CLogic::update_device_settings_from_server()
{
	if (false == _need_update_device_settings)
		return;
	
	CSettingsWorkState* sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
	logic::tag_device_settings work_settings = sws->get_settings();
	bool changed = false;

	if (work_settings.bill_acceptor_impulse != _settings_from_server.bill_acceptor_impulse)
	{
		work_settings.bill_acceptor_impulse = _settings_from_server.bill_acceptor_impulse;
		_common_settings.SetBillAcceptorImpulse(_settings_from_server.bill_acceptor_impulse);
		changed = true;
	}
	if (work_settings.coin_acceptor_impulse != _settings_from_server.coin_acceptor_impulse)
	{
		work_settings.coin_acceptor_impulse = _settings_from_server.coin_acceptor_impulse;
		_common_settings.SetCoinAcceptorImpulse(_settings_from_server.coin_acceptor_impulse);
		changed = true;
	}
	if (work_settings.free_idle_time != _settings_from_server.free_idle_time)
	{
		work_settings.free_idle_time = _settings_from_server.free_idle_time;
		_common_settings.SetFreeIdleTime(_settings_from_server.free_idle_time);
		changed = true;
	}
	if (work_settings.idle_time_cost != _settings_from_server.idle_time_cost)
	{
		work_settings.idle_time_cost = _settings_from_server.idle_time_cost;
		_common_settings.SetIdleTimeCost(_settings_from_server.idle_time_cost);
		changed = true;
	}
	if (work_settings.pause_before_advertising != _settings_from_server.pause_before_advertising)
	{
		work_settings.pause_before_advertising = _settings_from_server.pause_before_advertising;
		_common_settings.SetPauseBeforeAdvertising(_settings_from_server.pause_before_advertising);
		changed = true;
	}
	if (work_settings.state != static_cast<uint32_t>(_settings_from_server.state))
	{
		work_settings.state = static_cast<uint32_t>(_settings_from_server.state);
		_common_settings.SetState(static_cast<uint32_t>(_settings_from_server.state));
		changed = true;

		_terminal_state_changed_sended = false;
		change_terminal_state(_settings_from_server.state, true);
	}

	if (work_settings.frost_protection_value != _settings_from_server.frost_protection_value)
	{
		work_settings.frost_protection_value = _settings_from_server.frost_protection_value;
		_common_settings.SetFrostProtectionValue(_settings_from_server.frost_protection_value);
		changed = true;
	}

	if (work_settings.discount_card_condition != _settings_from_server.discount_card_condition)
	{
		work_settings.discount_card_condition = _settings_from_server.discount_card_condition;
		_common_settings.SetDiscountCardCondition(_settings_from_server.discount_card_condition);
		changed = true;
	}

	if (work_settings.cost_air != _settings_from_server.air)
	{
		work_settings.cost_air = _settings_from_server.air;
		_correspond_settings.SetServiceCost(e_service_name::air, static_cast<uint16_t>(_settings_from_server.air));
		changed = true;
	}
	if (work_settings.cost_foam != _settings_from_server.foam)
	{
		work_settings.cost_foam = _settings_from_server.foam;
		_correspond_settings.SetServiceCost(e_service_name::foam, static_cast<uint16_t>(_settings_from_server.foam));
		changed = true;
	}
	if (work_settings.cost_osmosis != _settings_from_server.osmosis)
	{
		work_settings.cost_osmosis = _settings_from_server.osmosis;
		_correspond_settings.SetServiceCost(e_service_name::osmosis, static_cast<uint16_t>(_settings_from_server.osmosis));
		changed = true;
	}
	if (work_settings.cost_pressurized_water != _settings_from_server.pressurized_water)
	{
		work_settings.cost_pressurized_water = _settings_from_server.pressurized_water;
		_correspond_settings.SetServiceCost(e_service_name::pressurized_water, static_cast<uint16_t>(_settings_from_server.pressurized_water));
		changed = true;
	}
	if (work_settings.cost_vacuum_cleaner != _settings_from_server.vacuum_cleaner)
	{
		work_settings.cost_vacuum_cleaner = _settings_from_server.vacuum_cleaner;
		_correspond_settings.SetServiceCost(e_service_name::vacuum_cleaner, static_cast<uint16_t>(_settings_from_server.vacuum_cleaner));
		changed = true;
	}
	if (work_settings.cost_water_without_pressure != _settings_from_server.water_without_pressure)
	{
		work_settings.cost_water_without_pressure = _settings_from_server.water_without_pressure;
		_correspond_settings.SetServiceCost(e_service_name::water_without_pressure, static_cast<uint16_t>(_settings_from_server.water_without_pressure));
		changed = true;
	}
	if (work_settings.cost_wax != _settings_from_server.wax)
	{
		work_settings.cost_wax = _settings_from_server.wax;
		_correspond_settings.SetServiceCost(e_service_name::wax, static_cast<uint16_t>(_settings_from_server.wax));
		changed = true;
	}

	if (true == changed)
	{
		sws->set_settings(work_settings);
		sws->write_settings();
		set_state(e_state::settings_work);

		_common_settings.ReadSettings();
		_correspond_settings.ReadSettings();

		if (_on_service_info_readed)
			send_services_info();
	}

	_need_update_device_settings = false;
}

void logic::CLogic::distribute_prize()
{
	if (false == _need_distribute_prize)
		return;

	CDistributionOfPrizeState* dps = get_implemented_state<CDistributionOfPrizeState>(e_state::distribution_of_prize);
	dps->distribute(_prize_packet.prize_size);

	set_state(e_state::distribution_of_prize);

	_need_distribute_prize = false;
}

void logic::CLogic::open_valve(byte number)
{
	std::shared_ptr<logic_structures::tag_open_valve> ov_message = std::make_shared<logic_structures::tag_open_valve>() ;

	ov_message->number = number;

	_packets_to_device.push_back(ov_message);
}

void logic::CLogic::close_valve(byte number)
{
	std::shared_ptr<logic_structures::tag_close_valve> cv_message = std::make_shared<logic_structures::tag_close_valve>();

	cv_message->number = number;

	_packets_to_device.push_back(cv_message);
}

void logic::CLogic::SetOnTimeAndMoneyFn(std::function<void(int16_t, int16_t) > fn)
{
	_on_change_time_and_money_fn = fn;
}

void logic::CLogic::SetOnServiceChangedFn(std::function<void(e_service_name, std::wstring) > fn)
{
	_on_service_changed_fn = fn;
}

void logic::CLogic::SetOnStateChangedFn(std::function<void(e_state)> fn)
{
	_on_state_changed_fn = fn;
}

void logic::CLogic::read_eeprom(byte cell_number)
{
	std::shared_ptr<logic_structures::tag_read_eeprom> re_message = std::make_shared<logic_structures::tag_read_eeprom>();

	re_message->cell_number = cell_number;

	_packets_to_device.push_back(re_message);
}

void logic::CLogic::write_eeprom(byte cell_number, uint32_t value)
{
	std::shared_ptr<logic_structures::tag_write_eeprom> we_message = std::make_shared<logic_structures::tag_write_eeprom>();

	we_message->cell_number = cell_number;
	we_message->value = value;

	_packets_to_device.push_back(we_message);
}

void logic::CLogic::time_and_money(int16_t time, int16_t money)
{
	_str_str.str(std::wstring());

	_str_str << _T("Осталось времени: ") << time << _T(" сек.") << _T("\r\n") <<
		_T("Осталось денег на счёте: ") << static_cast<float>(money / 100.0) << std::endl;

	_tr_error->trace_message(_str_str.str());

	if (_on_change_time_and_money_fn)
		_on_change_time_and_money_fn(time, money);
}

void logic::CLogic::set_state(e_state state)
{
	if (_current_state)
		_current_state->deactivate();

	_current_state = get_state(state);

	// если переходим в режим рекламного простоя и 
	// есть настройки с сервера, то обновляем настройки устройства
	if (e_state::advertising_idle == _current_state->get_this_state())
	{
		update_device_settings_from_server();
		distribute_prize();
	}

	if (_on_state_changed_fn)
		_on_state_changed_fn(state);

	_current_state->activate();
}

void logic::CLogic::process_messages_from_device()
{
	std::vector<std::shared_ptr<logic_structures::tag_base_data_from_device>> messages = 
		_packets_from_device.get_with_cleanup();

	for (std::shared_ptr<logic_structures::tag_base_data_from_device> message : messages)
	{
		process_device_message(message);
	}
}

void logic::CLogic::process_device_message(std::shared_ptr<logic_structures::tag_base_data_from_device> message)
{
	byte button_number = 0;
	e_service_name service = e_service_name::stop;
	CSettingsWorkState* sws = nullptr;
	logic_structures::tag_data_from_eeprom* dfe = nullptr;
	CDistributionOfPrizeState* dws = nullptr;
	logic_structures::tag_hopper_issue_coin* hic = nullptr;
	logic_structures::tag_buttons_state* bs = nullptr;
	CAdvertisingIdleState* ais = nullptr;
	logic_structures::tag_command_confirmation* cc = nullptr;
	logic_structures::tag_discount_card_issued* dci = nullptr;

	switch (message->command_id)
	{
		case(device_exchange::e_command_from_device::bill_acceptor) :
			_current_state->refilled_cache();
			sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
			break;

		case(device_exchange::e_command_from_device::button_press) :
			button_number = get_device_message_pointer<logic_structures::tag_button_press>(message)->button_number;
			service = _correspond_settings.GetServiceByButtonNumber(button_number);
			if (e_service_name::error == service)
				break;
			if (_on_service_changed_fn)
				_on_service_changed_fn(service, _correspond_settings.GetServiceName(service));
			if (e_service_name::stop == service)
				_current_state->stop_button_press();
			else
				_current_state->service_button_press(service);
			break;

		case(device_exchange::e_command_from_device::data_from_eeprom) :
			dfe = get_device_message_pointer<logic_structures::tag_data_from_eeprom>(message);
			sws = get_implemented_state<CSettingsWorkState>(e_state::settings_work);
			sws->data_from_eeprom(dfe->cell_number, dfe->value);
			break;

		case(device_exchange::e_command_from_device::hopper_issue_coin) :
			hic = get_device_message_pointer<logic_structures::tag_hopper_issue_coin>(message);
			dws = get_implemented_state<CDistributionOfPrizeState>(e_state::distribution_of_prize);
			dws->coin_issued(hic->balance);
			break;

		case(device_exchange::e_command_from_device::command_confirmation) :
			cc = get_device_message_pointer<logic_structures::tag_command_confirmation>(message);
			_current_state->device_confirm(cc->command);
			break;

		case(device_exchange::e_command_from_device::buttons_state) :
			bs = get_device_message_pointer<logic_structures::tag_buttons_state>(message);
			if (e_state::advertising_idle == _current_state->get_this_state())
			{
				ais = get_implemented_state<CAdvertisingIdleState>(e_state::advertising_idle);
				ais->buttons_state(*bs);
			}
			break;

		case(device_exchange::e_command_from_device::discount_card_issued) :
			dci = get_device_message_pointer<logic_structures::tag_discount_card_issued>(message);
			if (logic_structures::e_discount_card_issue_status::previous_card_not_taken == dci->status)
			{
				send_log_to_server(server_exchange::e_log_record_type::warning, _T("Дисконтная карта не выдана, т.к. не забрали предыдущую карту"));
			}
			else if (logic_structures::e_discount_card_issue_status::error == dci->status)
			{
				send_log_to_server(server_exchange::e_log_record_type::warning, _T("Ошибка выдачи дисконтной карты. Возможно, карты закончились."));
			}
			else
			{
				send_log_to_server(server_exchange::e_log_record_type::message, _T("Дисконтная карта успешно выдана."));
			}
			break;
	}
}

std::shared_ptr<logic::IState> logic::CLogic::get_state(e_state state)
{
	return _states[state];
}
