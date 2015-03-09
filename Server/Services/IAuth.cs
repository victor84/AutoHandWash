using System;
namespace Server.Services
{
    public interface IAuth
    {
        User GetUserById(Guid id);
        User GetUserByName(string name);
    }
}
