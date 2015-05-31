using Microsoft.AspNet.SignalR;
using Server.Data;
using System;

namespace Server.Hubs
{
    public class MainHub : Hub
    {
        public void RefreshCounters(string terminalName, Counters counters)
        {
            Clients.All.RefreshCounters(terminalName, counters);
        }

        public void RefreshStatusBar(Guid groupId, string message)
        {
            Clients.All.RefreshStatusBar(groupId, message);
        }
    }
}
