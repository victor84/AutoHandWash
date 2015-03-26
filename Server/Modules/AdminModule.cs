using Nancy;
using Nancy.Security;
using Server.Data;
using Server.Models;
using System.Collections.Generic;
using System.Linq;

namespace Server.Modules
{
    public class AdminModule : BaseModule
    {
        public AdminModule()
            : base("admin")
        {
            this.RequiresAuthentication();
            this.RequiresClaims(new string[] { User.adminClaim });
            Get["/"] = Index;
            Get["/userGroups"] = ViewUserGroups;
            Get["/userGroups/create"] = ViewCreateUserGroups;
            Post["/userGroups/create"] = CreateUserGroups;
        }

        private dynamic Index(dynamic parameters)
        {
            return View["AdminIndex", Model];
        }

        private dynamic ViewUserGroups(dynamic parameters)
        {
            List<UserGroupNamePair> listUserGroups = new List<UserGroupNamePair>();
            var userGroups = UserGroups.GetUserGroups();
            if (userGroups != null)
            {
                var users = User.GetUsers();
                var groups = Group.GetGroups();
                if (users != null && groups != null)
                {
                    foreach (var userGroup in userGroups)
                    {
                        var userId = userGroup.UserId;
                        var groupId = userGroup.GroupId;

                        var user = users.Where(x => x.Id == userId).FirstOrDefault();
                        var group = groups.Where(x => x.Id == groupId).FirstOrDefault();

                        if (user != null && group != null)
                        {
                            UserGroupNamePair pair = new UserGroupNamePair() 
                            { 
                                UserName = user.UserName,
                                GroupName = group.GroupName
                            };
                            listUserGroups.Add(pair);
                        }
                    }
                }
            }
            Model.AdminPage = new AdminPageModel();
            Model.AdminPage.UserGroups = listUserGroups;
            return View["UserGroups", Model];
        }

        private dynamic ViewCreateUserGroups(dynamic parameters)
        {
            var users = User.GetUsers();
            var groups = Group.GetGroups();
            if (users != null && groups != null)
            {
                Model.AdminPage = new AdminPageModel();
                Model.AdminPage.Users = users;
                Model.AdminPage.Groups = groups;
            }
            return View["CreateUserGroups", Model];
        }

        private dynamic CreateUserGroups(dynamic parameters)
        {
            var userName = (string)this.Request.Form.userName;
            var groupName = (string)this.Request.Form.groupName;
            var user = User.GetUserByName(userName);
            var group = Group.GetGroupByName(groupName);
            if (user != null && group != null)
            {
                UserGroups newUserGroups = new UserGroups()
                {
                    UserId = user.Id,
                    GroupId = group.Id
                };
                var userGroup = UserGroups.GetUserGroup(newUserGroups);
                if (userGroup == null)
                    UserGroups.Insert(newUserGroups);
            }
            return Response.AsRedirect("~/admin/userGroups");
        }
    }
}
