using Microsoft.AspNet.SignalR;
using Server.Data;

namespace Server.Hubs
{
    public class MainHub : Hub
    {
        public void RefreshCounters(string terminalName, Counters counters)
        {
            Clients.All.RefreshCounters(terminalName, counters);
        }
    }
}
