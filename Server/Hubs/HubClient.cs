﻿using Microsoft.AspNet.SignalR.Client;
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
        
        public HubClient(ushort port)
        {
            lockInvoke = new object();
            string url = "http://localhost:" + port.ToString();
            Connection = new HubConnection(url);
            Proxy = Connection.CreateHubProxy(hubName);
        }

        public Task Invoke(string method, params Object[] args)
        {
            Task task = null;
            lock (lockInvoke)
            {
                try 
                {
                    if (Connection.State == ConnectionState.Disconnected)
                    {
                        Connection.Start().Wait();
                    }
                    task = Proxy.Invoke(method, args);
                }
                catch (Exception e)
                {
                    ServerLogger.Error(string.Format("HubClient -> Invoke: {0}", e.Message));
                }
            }
            return task;
        }

        public Task<T> Invoke<T>(string method, params Object[] args)
        {
            Task<T> task = null;
            lock (lockInvoke)
            {
                try 
                {
                    if (Connection.State == ConnectionState.Disconnected)
                    {
                        Connection.Start().Wait();
                    }
                    task = Proxy.Invoke<T>(method, args);
                }
                catch (Exception e)
                {
                    ServerLogger.Error(string.Format("HubClient -> Invoke<T>: {0}", e.Message));
                }
            }
            return task;
        }
    }
}
