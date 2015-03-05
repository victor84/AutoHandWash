using Nancy.Security;
namespace Server.Modules
{
    public class MainModule : BaseModule
    {
        public MainModule()
        {
            this.RequiresAuthentication();
            Get["/groups"] = TestSignalR;
        }

        private dynamic TestSignalR(dynamic parameters)
        {
            return View["TestSignalR", Model];
        }
    }
}
