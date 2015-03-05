using System;
using Nancy;
using Nancy.Authentication.Forms;

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

            // TODO: Здесь надо сделать выборку пользователя из БД, используя его имя.
            // var user = _dbService.GetByUserName(name);
            // TODO: Проверку на валидность пользователя
            Guid userIdentifier = new Guid("52AC6CED-995A-4A42-8E06-50058BF91B19");
            return this.LoginAndRedirect(userIdentifier);
        }

        private dynamic Logout(dynamic parameters)
        {
            return this.LogoutAndRedirect("/");
        }
    }
}
