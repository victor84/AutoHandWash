using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Parsing;

namespace ParsingTest
{
    class Program
    {
        static void Main(string[] args)
        {
            PacketToRawData packet_to_raw_data = new PacketToRawData();

            tag_transport_packet tp;
            tag_identification_packet data;
            data.group_name = "aaaaaaaaaaaaaaaaaaaa".ToCharArray();
            data.terminal_name = "zzzzzzzzzzzzzzzzzzzz".ToCharArray();

            tp.begin = tag_constants.begin_bytes;
            tp.type = e_packet_type.id;
            tp.length = 40;
            packet_to_raw_data.CreateIdentificationPacketRawData(data, out tp.data);
            tp.end = tag_constants.end_bytes;

            Byte[] raw_data = null;

            packet_to_raw_data.CreateRawData(tp, out raw_data);

            PacketParser parser = new PacketParser();

            IEnumerable<tag_transport_packet> parsed_tp;
            tag_identification_packet parsed_ip;

            parser.ParseTransportPacket(raw_data, out parsed_tp);
            parser.ParseIdentificationPacket(parsed_tp.ElementAt(0), out parsed_ip);

            tag_counters_packet counters_packet;
            counters_packet.date_time = (UInt64)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
            counters_packet.against_midges = 10;
            counters_packet.air = 20;
            counters_packet.current_cache = 100;
            counters_packet.foam = 30;
            counters_packet.osmosis = 80;
            counters_packet.pressurized_water = 40;
            counters_packet.state = e_terminal_state.work;
            counters_packet.total_cache = 2000;
            counters_packet.vacuum_cleaner = 50;
            counters_packet.water_without_pressure = 60;
            counters_packet.wax = 70;

            tag_transport_packet tp1;
            tp1.begin = tag_constants.begin_bytes;
            tp1.type = e_packet_type.counters;
            tp1.end = tag_constants.end_bytes;
            packet_to_raw_data.CreateCountersPacketRawData(counters_packet, out tp1.data);
            tp1.length = (UInt32)tp1.data.Length;

            packet_to_raw_data.CreateRawData(tp1, out raw_data);

            counters_packet = new tag_counters_packet();

            parser.ParseTransportPacket(raw_data, out parsed_tp);
            parser.ParseCountersPacket(parsed_tp.ElementAt(0), out counters_packet);

            tag_settings_packet settings_packet;
            settings_packet.state = e_terminal_state.work;
            settings_packet.bill_acceptor_impulse = 10;
            settings_packet.coin_acceptor_impulse = 11;
            settings_packet.free_idle_time = 3;
            settings_packet.idle_time_cost = 10;
            settings_packet.osmosis = 80;
            settings_packet.pause_before_advertising = 1;
            settings_packet.pressurized_water = 10;
            settings_packet.water_without_pressure = 20;
            settings_packet.foam = 30;
            settings_packet.wax = 40;
            settings_packet.against_midges = 50;
            settings_packet.vacuum_cleaner = 60;
            settings_packet.air = 70;

            tag_transport_packet tp2;
            tp2.begin = tag_constants.begin_bytes;
            tp2.type = e_packet_type.settings;
            tp2.end = tag_constants.end_bytes;
            packet_to_raw_data.CreateSettingsPacketRawData(settings_packet, out tp2.data);
            tp2.length = (UInt32)tp2.data.Length;

            packet_to_raw_data.CreateRawData(tp2, out raw_data);

            settings_packet = new tag_settings_packet();

            parser.ParseTransportPacket(raw_data, out parsed_tp);
            parser.ParseSettingsPacket(parsed_tp.ElementAt(0), out settings_packet);

            tag_confirmation_packet confirmation_packet;
            confirmation_packet.packet_type = e_packet_type.counters;
            confirmation_packet.result = e_processing_result.success;

            tag_transport_packet tp3;
            tp3.begin = tag_constants.begin_bytes;
            tp3.type = e_packet_type.confirmation;
            tp3.end = tag_constants.end_bytes;
            packet_to_raw_data.CreateConfirmationPacketRawData(confirmation_packet, out tp3.data);
            tp3.length = (UInt32)tp3.data.Length;

            packet_to_raw_data.CreateRawData(tp3, out raw_data);

            confirmation_packet = new tag_confirmation_packet();

            parser.ParseTransportPacket(raw_data, out parsed_tp);
            parser.ParseConfirmationPacket(parsed_tp.ElementAt(0), out confirmation_packet);


            tag_log_record_packet log_record_packet;
            log_record_packet.date_time = (UInt64)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
            log_record_packet.type = e_log_record_type.message;

            Char[] char_str = "Это сообщение для записи в лог".ToCharArray();

            log_record_packet.text = char_str;
            log_record_packet.length = (UInt16)(log_record_packet.text.Length * 2);

            tag_transport_packet tp4;
            tp4.begin = tag_constants.begin_bytes;
            tp4.type = e_packet_type.log;
            tp4.end = tag_constants.end_bytes;
            packet_to_raw_data.CreateLogRecordPacketRawData(log_record_packet, out tp4.data);
            tp4.length = (UInt32)tp4.data.Length;

            packet_to_raw_data.CreateRawData(tp4, out raw_data);

            log_record_packet = new tag_log_record_packet();

            parser.ParseTransportPacket(raw_data, out parsed_tp);
            parser.ParseLogRecordPacket(parsed_tp.ElementAt(0), out log_record_packet);



        }
    }
}
