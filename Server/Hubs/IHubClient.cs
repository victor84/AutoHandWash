using System;
using System.Threading.Tasks;

namespace Server.Hubs
{
    public interface IHubClient
    {
        Task Invoke(string method, params Object[] args);

        Task<T> Invoke<T>(string method, params Object[] args);
    }
}
