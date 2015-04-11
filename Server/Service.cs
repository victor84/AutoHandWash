using Microsoft.Owin.Hosting;
using Server.Hubs;
using Server.Pipes;
using System;

namespace Server
{
    public class Service
    {
        private IDisposable webApp;
        private TcpServer tcpServer;
        private PipeServer pipeServer;
        private HubClient hubClient;
        private AppSettings appSettings;
        
        public Service(AppSettings appSettings)
        {
            this.appSettings = appSettings;
            hubClient = new HubClient(appSettings.BaseUri);
            tcpServer = new TcpServer(appSettings.Port, hubClient);
            pipeServer = new PipeServer(tcpServer.PipeMessageReceived);
        }

        public void Start()
        {
            webApp = WebApp.Start<Startup>(appSettings.BaseUri);
            tcpServer.Start();
            pipeServer.Start();
        }

        public void Stop()
        {
            pipeServer.Stop();
            tcpServer.Stop();
            webApp.Dispose();
        }
    }
}
