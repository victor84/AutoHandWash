using Microsoft.AspNet.SignalR.Client;
using System;
using System.Threading.Tasks;

namespace Server.Hubs
{
    public class HubClient : IHubClient
    {
        private string hubName = "mainHub";
        private object lockInvoke;
        private HubConnection Connection { get; set; }
        private IHubProxy Proxy { get; set; }
        
        public HubClient(string url)
        {
            lockInvoke = new object();
            Connection = new HubConnection(url);
            Proxy = Connection.CreateHubProxy(hubName);
        }

        public void Start()
        {
            Connection.Start().Wait();
        }

        public void Stop()
        {
            //Connection.Stop();
        }

        public Task Invoke(string method, params Object[] args)
        {
            Task task = null;
            lock (lockInvoke)
            {
                task = Proxy.Invoke(method, args);
            }
            return task;
        }

        public Task<T> Invoke<T>(string method, params Object[] args)
        {
            Task<T> task = null;
            lock (lockInvoke)
            {
                task = Proxy.Invoke<T>(method, args);
            }
            return task;
        }
    }
}
