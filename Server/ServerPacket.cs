using Parsing;
using System;

namespace Server
{
    [Serializable]
    public class ServerPacket
    {
        public Guid Id { get; set; }
        public ServerPacketType PacketType { get; set; }
        public object Data { get; set; }
        public ServerPacket(Guid id, ServerPacketType packetType, object data)
        {
            Id = id;
            PacketType = packetType;
            Data = data;
        }
    }
}
