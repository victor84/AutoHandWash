using Parsing;
using System;

namespace Server
{
    [Serializable]
    public class ServerPacket
    {
        public e_packet_type PacketType { get; set; }
        public object Data { get; set; }
        public ServerPacket(e_packet_type packetType, object data)
        {
            PacketType = packetType;
            Data = data;
        }
    }
}
