using Microsoft.AspNet.SignalR;

namespace Server.Hubs
{
    public class TestHub : Hub
    {
        public void Send(string name, string message)
        {
            Clients.All.addMessage(name, message);
        }
    }
}
