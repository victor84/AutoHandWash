using Microsoft.AspNet.SignalR;
using Server.Data;
using System;

namespace Server.Hubs
{
    public class MainHub : Hub
    {
        public void RefreshCounters(string terminalName, Counters counters, long sumPrizeCounters, int sumDiscountCardCounters)
        {
            Clients.All.RefreshCounters(terminalName, counters, sumPrizeCounters, sumDiscountCardCounters);
        }

        public void RefreshStatusBar(Guid groupId, string message)
        {
            Clients.All.RefreshStatusBar(groupId, message);
        }

        public void RefreshMessages(Guid groupId, string terminalName, TerminalLog terminalLog)
        {
            Clients.All.RefreshMessages(groupId, terminalName, terminalLog);
        }
    }
}
