using Nancy.Extensions;
using Nancy.Authentication.Forms;
using Server.Services;
using System.Linq;

namespace Server.Modules
{
    public class LoginModule : BaseModule
    {
        private readonly IAuth _auth;
        
        public LoginModule(IAuth auth)
        {
            _auth = auth;
            
            Get["/"] = Index;
            Get["/login"] = Login;
            Post["/login"] = DoLogin;
            Post["/logout"] = Logout;
        }

        private dynamic Index(dynamic parameters)
        {
            return View["Index", Model];
        }

        private dynamic Login(dynamic parameters)
        {
            return View["Login", Model];
        }

        private dynamic DoLogin(dynamic parameters)
        {
            var username = (string)this.Request.Form.Username;
            var password = (string)this.Request.Form.Password;
            var user = _auth.GetUserByName(username);
            if (user == null || user.Password != password)
            {
                return Context.GetRedirect("~/login?error=true&username=" + username);
            }
            return this.LoginAndRedirect(user.Id);
        }

        private dynamic Logout(dynamic parameters)
        {
            return this.LogoutAndRedirect("/");
        }
    }
}
