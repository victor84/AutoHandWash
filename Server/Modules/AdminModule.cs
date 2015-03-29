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
            Get["/groups/settings/{groupName}"] = ViewSettingsGroup;
            Post["/groups/settings"] = ChangeSettingsGroup;
            Get["/terminals"] = ViewTerminals;
            Get["/terminals/settings/{groupName}/{terminalName}"] = ViewSettingsTerminals;
            Post["/terminals/settings"] = ChangeSettingsTerminal;

            Get["/error/{type}"] = ViewError;
        }

        private dynamic Index(dynamic parameters)
        {
            return View["AdminIndex", Model];
        }

        private dynamic ViewUsers(dynamic parameters)
        {
            Model.AdminPage = new AdminPageModel();
            var users = User.GetUsers();
            if (users != null)
            {
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
                    if (!result)
                    {
                        return Response.AsRedirect("~/admin/error/" + (byte)AdminErrors.CreateUser);
                    }
                }
                else
                {
                    return Response.AsRedirect("~/admin/error/" + (byte)AdminErrors.UserNameExist);
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

        private dynamic ViewSettingsGroup(dynamic parameters)
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
            return View["SettingsGroup", Model];
        }

        private dynamic ChangeSettingsGroup(dynamic parameters)
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

        private dynamic ViewTerminals(dynamic parameters)
        {
            List<GroupTerminalNamePair> listGroupTerminals = new List<GroupTerminalNamePair>();
            var terminals = Terminal.GetTerminals();
            var groups = Group.GetGroups();
            if (terminals != null && groups != null)
            {
                foreach (var terminal in terminals)
                {
                    string groupName = string.Empty;
                    var group = groups.Where(x => x.Id == terminal.GroupId).FirstOrDefault();
                    if (group != null)
                    {
                        groupName = group.GroupName;
                    }
                    GroupTerminalNamePair pair = new GroupTerminalNamePair()
                    {
                        GroupName = groupName,
                        TerminalName = terminal.TerminalName
                    };
                    listGroupTerminals.Add(pair);
                }
            }
            Model.AdminPage = new AdminPageModel();
            Model.AdminPage.GroupTerminals = listGroupTerminals;
            return View["AdminTerminals", Model];
        }

        private dynamic ViewSettingsTerminals(dynamic parameters)
        {
            var groupName = (string)parameters.groupName;
            var terminalName = (string)parameters.terminalName;
            if (!string.IsNullOrEmpty(groupName) && !string.IsNullOrEmpty(terminalName))
            {
                Model.AdminPage = new AdminPageModel();
                Model.AdminPage.GroupName = groupName;
                Model.AdminPage.TerminalName = terminalName;
                var group = Group.GetGroupByName(groupName);
                var terminals = Terminal.GetTerminalsByName(terminalName);
                if (group != null && terminals != null)
                {
                    var terminal = terminals.Where(x => x.GroupId == group.Id).FirstOrDefault();
                    if (terminal != null)
                    {
                        var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminal.Id);
                        if (settingsTerminal != null)
                        {
                            Model.AdminPage.SettingsTerminal = settingsTerminal;
                        }
                    }
                }
            }
            return View["SettingsTerminal", Model];
        }

        private dynamic ChangeSettingsTerminal(dynamic parameters)
        {
            var groupName = (string)this.Request.Form.GroupName;
            var terminalName = (string)this.Request.Form.TerminalName;
            var state = (byte)this.Request.Form.State;
            var impulseBillAcceptor = (uint)this.Request.Form.ImpulseBillAcceptor;
            var impulseCoinAcceptor = (uint)this.Request.Form.ImpulseCoinAcceptor;
            var timeInactivity = (uint)this.Request.Form.TimeInactivity;
            var priceMinuteInactivity = (uint)this.Request.Form.PriceMinuteInactivity;
            var pauseBeforeShowingAds = (uint)this.Request.Form.PauseBeforeShowingAds;
            var pricePressurizedWater = (uint)this.Request.Form.PricePressurizedWater;
            var priceNoPressurizedWater = (uint)this.Request.Form.PriceNoPressurizedWater;
            var priceFoam = (uint)this.Request.Form.PriceFoam;
            var priceWax = (uint)this.Request.Form.PriceWax;
            var priceAgainstOfMidges = (uint)this.Request.Form.PriceAgainstOfMidges;
            var priceVacuum = (uint)this.Request.Form.PriceVacuum;
            var priceAir = (uint)this.Request.Form.PriceAir;
            var priceOsmose = (uint)this.Request.Form.PriceOsmose;

            if (!string.IsNullOrEmpty(groupName) && !string.IsNullOrEmpty(terminalName))
            {
                var group = Group.GetGroupByName(groupName);
                if (group != null)
                {
                    var terminals = Terminal.GetTerminalsByName(terminalName);
                    if (terminals != null)
                    {
                        var terminal = terminals.Where(x => x.GroupId == group.Id).FirstOrDefault();
                        if (terminal != null)
                        {
                            var newSettingsTerminal = new SettingsTerminal()
                            {
                                TerminalId = terminal.Id,
                                State = state,
                                ImpulseBillAcceptor = impulseBillAcceptor,
                                ImpulseCoinAcceptor = impulseCoinAcceptor,
                                PauseBeforeShowingAds = pauseBeforeShowingAds,
                                PriceAgainstOfMidges = priceAgainstOfMidges,
                                PriceAir = priceAir,
                                PriceFoam = priceFoam,
                                PriceMinuteInactivity = priceMinuteInactivity,
                                PriceNoPressurizedWater = priceNoPressurizedWater,
                                PriceOsmose = priceOsmose,
                                PricePressurizedWater = pricePressurizedWater,
                                PriceVacuum = priceVacuum,
                                PriceWax = priceWax,
                                TimeInactivity = timeInactivity
                            };
                            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminal.Id);
                            if (settingsTerminal == null)
                            {
                                SettingsTerminal.Insert(newSettingsTerminal);
                            }
                            else
                            {
                                SettingsTerminal.Update(newSettingsTerminal);
                            }
                        }
                    }
                }
            }
            return Response.AsRedirect("~/admin/terminals");
        }

        private dynamic ViewError(dynamic parameters)
        {
            var type = (byte)parameters.type;
            Model.AdminPage = new AdminPageModel();
            Model.AdminPage.AdminError = new AdminError();
            Model.AdminPage.AdminError.Type = (AdminErrors)type;
            return View["AdminError", Model];
        }
    }
}
