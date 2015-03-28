using Nancy;
using Nancy.Security;
using Server.Data;
using Server.Models;
using System;
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
            Get["/users"] = ViewUsers;
            Get["/users/create"] = ViewCreateUser;
            Post["/users/create"] = CreateUser;
            Get["/users/delete/{userName}"] = DeleteUser;
            Get["/userGroups"] = ViewUserGroups;
            Get["/userGroups/create"] = ViewCreateUserGroups;
            Post["/userGroups/create"] = CreateUserGroups;
            Get["/userGroups/delete/{userName}/{groupName}"] = DeleteUserGroups;
            Get["/groups"] = ViewGroups;
            Get["/groups/setting/{groupName}"] = ViewSettingGroup;
            Post["/groups/setting"] = SettingGroup;
        }

        private dynamic Index(dynamic parameters)
        {
            return View["AdminIndex", Model];
        }

        private dynamic ViewUsers(dynamic parameters)
        {
            var users = User.GetUsers();
            if (users != null)
            {
                Model.AdminPage = new AdminPageModel();
                Model.AdminPage.Users = users;
            }
            return View["Users", Model];
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

        private dynamic ViewCreateUser(dynamic parameters)
        {
            return View["CreateUser", Model];
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

        private dynamic CreateUser(dynamic parameters)
        {
            var userName = (string)this.Request.Form.Username;
            var password = (string)this.Request.Form.Password;
            if (!string.IsNullOrEmpty(userName) && !string.IsNullOrEmpty(password))
            {
                var user = User.GetUserByName(userName);
                if (user == null)
                {
                    User newUser = new User()
                    {
                        Id = Guid.NewGuid(),
                        UserName = userName,
                        Password = password,
                        Claim = User.userClaim,
                    };
                    bool result = User.Insert(newUser);
                }
            }
            return Response.AsRedirect("~/admin/users");
        }

        private dynamic DeleteUser(dynamic parameters)
        {
            var userName = (string)parameters.userName;
            if (!string.IsNullOrEmpty(userName))
            {
                var user = User.GetUserByName(userName);
                if (user != null)
                {
                    bool result = User.Delete(user);
                }
            }
            return Response.AsRedirect("~/admin/users");
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

        private dynamic DeleteUserGroups(dynamic parameters)
        {
            var userName = (string)parameters.userName;
            var groupName = (string)parameters.groupName;
            if (!string.IsNullOrEmpty(userName) && !string.IsNullOrEmpty(groupName))
            {
                var user = User.GetUserByName(userName);
                var group = Group.GetGroupByName(groupName);

                if (user != null && group != null)
                {
                    UserGroups userGroups = new UserGroups()
                    {
                        UserId = user.Id,
                        GroupId = group.Id
                    };
                    var userGroup = UserGroups.GetUserGroup(userGroups);
                    if (userGroup != null)
                        UserGroups.Delete(userGroups);
                }
            }
            return Response.AsRedirect("~/admin/userGroups");
        }

        private dynamic ViewGroups(dynamic parameters)
        {
            var groups = Group.GetGroups();
            if (groups != null)
            {
                Model.AdminPage = new AdminPageModel();
                Model.AdminPage.Groups = groups;
            }
            return View["Groups", Model];
        }

        private dynamic ViewSettingGroup(dynamic parameters)
        {
            var groupName = (string)parameters.groupName;
            if (!string.IsNullOrEmpty(groupName))
            {
                Model.AdminPage = new AdminPageModel();
                Model.AdminPage.GroupName = groupName;
                var group = Group.GetGroupByName(groupName);
                if (group != null)
                {
                    var settingsGroup = SettingsGroup.GetSettingsGroupById(group.Id);
                    if (settingsGroup != null)
                    {
                        Model.AdminPage.SettingsGroup = settingsGroup;
                    }
                }
            }
            return View["SettingGroup", Model];
        }

        private dynamic SettingGroup(dynamic parameters)
        {
            var groupName = (string)this.Request.Form.GroupName;
            var hasPresent = (bool)this.Request.Form.HasPresent;
            var percentForPresent = (byte)this.Request.Form.PercentForPresent;
            var lowerBoundForRandomSum = (uint)this.Request.Form.LowerBoundForRandomSum;
            var upperBoundForRandomSum = (uint)this.Request.Form.UpperBoundForRandomSum;
            
            if (!string.IsNullOrEmpty(groupName))
            {
                var group = Group.GetGroupByName(groupName);
                if (group != null)
                {
                    var newSettingsGroup = new SettingsGroup() 
                    {
                        GroupId = group.Id,
                        HasPresent = hasPresent,
                        LowerBoundForRandomSum = lowerBoundForRandomSum,
                        UpperBoundForRandomSum = upperBoundForRandomSum,
                        PercentForPresent = percentForPresent
                    };
                    var settingsGroup = SettingsGroup.GetSettingsGroupById(group.Id);
                    if (settingsGroup == null)
                    {
                        SettingsGroup.Insert(newSettingsGroup);
                    }
                    else
                    {
                        SettingsGroup.Update(newSettingsGroup);
                    }
                }
            }
            return Response.AsRedirect("~/admin/groups");
        }
    }
}
