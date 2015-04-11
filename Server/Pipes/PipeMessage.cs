using Parsing;
using System;

namespace Server.Pipes
{
    [Serializable]
    public class PipeMessage
    {
        public e_packet_type PacketType { get; set; }
        public object Message { get; set; }
        public PipeMessage(e_packet_type packetType, object message)
        {
            PacketType = packetType;
            Message = message;
        }
    }
}
