using Nancy.Extensions;
using Nancy.Authentication.Forms;
using Server.Services;
using System.Linq;
using Server.Models;

namespace Server.Modules
{
    public class LoginModule : BaseModule
    {   
        public LoginModule()
        {
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
            var user = User.GetUserByName(username);
            if (user == null || user.Password != password)
            {
                Model.LoginPage = new LoginPageModel();
                Model.LoginPage.IsError = true;
                return View["Login", Model];
            }
            return this.LoginAndRedirect(user.Id);
        }

        private dynamic Logout(dynamic parameters)
        {
            return this.LogoutAndRedirect("/");
        }
    }
}
