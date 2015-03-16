using Server.Data;
using System;

namespace Server.Services
{
    public interface ITerminals
    {
        Terminal GetTerminalById(Guid id);
        Terminal GetTerminalByName(string name);
    }
}
