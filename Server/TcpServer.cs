using Microsoft.AspNet.SignalR.Client;
using System.Threading;
using System.Threading.Tasks;

namespace Server
{
    public class TcpServer
    {
        private volatile bool running;
        private Task task;
        public TcpServer(string url, string hubName)
        {
            task = new Task(() => Main(url, hubName));
        }

        public void Start()
        {
            running = true;
            task.Start();
        }

        public void Stop()
        {
            running = false;
        }
            
        private void Main(string url, string hubName)
        {
            HubConnection hubConnection = null;
            IHubProxy hubProxy = null;
            while (running)
            {
                if (hubConnection == null || hubProxy == null)
                {
                    hubConnection = new HubConnection(url);
                    hubProxy = hubConnection.CreateHubProxy(hubName);
                    hubConnection.Start().Wait();
                }
                hubProxy.Invoke("Send", "Craig", "12345");
                Thread.Sleep(100);
            }
        }
    }
}
