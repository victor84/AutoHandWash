using Server.Data;
namespace Server.Hubs
{
    public class HubTerminals
    {
        private readonly IHubClient _hubClient;

        public HubTerminals(IHubClient hubClient)
        {
            _hubClient = hubClient;
        }

        public void RefreshCounters(string terminalName, Counters counters)
        {
            _hubClient.Invoke("RefreshCounters", terminalName, counters);
        }
    }
}
