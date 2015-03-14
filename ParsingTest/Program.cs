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

        }
    }
}
