namespace Server.Hubs
{
    public class HubTerminals
    {
        private readonly IHubClient _hubClient;

        public HubTerminals(IHubClient hubClient)
        {
            _hubClient = hubClient;
        }

        public void Send(string name, string message)
        {
            _hubClient.Invoke("Send", name, message);
        }
    }
}
