using Microsoft.Owin.Hosting;
using System;

namespace Server
{
    public class Service
    {
        private IDisposable webApp;
        private TcpServer tcpServer;
        private ServiceInfo serviceInfo;
        
        public Service(ServiceInfo serviceInfo)
        {
            this.serviceInfo = serviceInfo;
            tcpServer = new TcpServer(serviceInfo.Url, serviceInfo.HubName);
        }

        public void Start()
        {
            tcpServer.Start();
            webApp = WebApp.Start<Startup>(serviceInfo.Url);
        }

        public void Stop()
        {
            tcpServer.Stop();
            webApp.Dispose();
        }
    }
}
