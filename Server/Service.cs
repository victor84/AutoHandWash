using Microsoft.Owin.Hosting;
using Server.Hubs;
using System;

namespace Server
{
    public class Service
    {
        private IDisposable webApp;
        private TcpServer tcpServer;
        private HubClient hubClient;
        private AppSettings appSettings;
        
        public Service(AppSettings appSettings)
        {
            this.appSettings = appSettings;
            hubClient = new HubClient(appSettings.BaseUri, appSettings.HubName);
            tcpServer = new TcpServer(appSettings.Port, hubClient);
        }

        public void Start()
        {
            webApp = WebApp.Start<Startup>(appSettings.BaseUri);
            hubClient.Start();
            tcpServer.Start();
        }

        public void Stop()
        {
            tcpServer.Stop();
            hubClient.Stop();
            webApp.Dispose();
        }
    }
}
