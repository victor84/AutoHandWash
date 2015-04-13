using Parsing;
using System;

namespace Server
{
    [Serializable]
    public class ServerPacket
    {
        public Guid TerminalId { get; set; }
        public e_packet_type PacketType { get; set; }
        public object Data { get; set; }
        public ServerPacket(Guid terminalId, e_packet_type packetType, object data)
        {
            TerminalId = terminalId;
            PacketType = packetType;
            Data = data;
        }
    }
}
