using Server.Data;
using System.Collections.Generic;
using System.Linq;

namespace Server.Models
{
    public enum AdminErrors
    {
        UserNameExist,
        CreateUser,
        DeleteUser,
        UserGroupExist,
        CreateUserGroup,
        DeleteUserGroup,
        CreateSettingsGroup,
        UpdateSettingsGroup,
        ReadSettingsTerminals
    };
    
    public class AdminError
    {
        public AdminErrors Type { get; set; }
        public string Message 
        {
            get
            {
                string result = string.Empty;
                switch(Type)
                {
                    case AdminErrors.UserNameExist:
                        result = "Пользователь с таким именем уже существует";
                        break;
                    case AdminErrors.CreateUser:
                        result = "Не удалось создать пользователя";
                        break;
                    case AdminErrors.DeleteUser:
                        result = "Не удалось удалить пользователя";
                        break;
                    case AdminErrors.UserGroupExist:
                        result = "Подобная связь пользователя с группой уже существует";
                        break;
                    case AdminErrors.CreateUserGroup:
                        result = "Не удалось добавить связь пользователя с группой";
                        break;
                    case AdminErrors.DeleteUserGroup:
                        result = "Не удалось удалить связь пользователя с группой";
                        break;
                    case AdminErrors.CreateSettingsGroup:
                        result = "Не удалось добавить настройки группы";
                        break;
                    case AdminErrors.UpdateSettingsGroup:
                        result = "Не удалось обновить настройки группы";
                        break;
                    case AdminErrors.ReadSettingsTerminals:
                        result = "Не удалось прочитать настройки терминала";
                        break;
                    default:
                        break;
                }
                return result;
            }
        }
    }
    
    public class GroupTerminalNamePair
    {
        public string GroupName { get; set; }
        public string TerminalName { get; set; }
    }

    public class UserGroupNamePair
    {
        public string UserName { get; set; }
        public string GroupName { get; set; }
    }
    
    public class AdminPageModel
    {
        public List<User> Users { get; set; }
        public List<Group> Groups { get; set; }
        public List<UserGroupNamePair> UserGroups { get; set; }
        public List<GroupTerminalNamePair> GroupTerminals { get; set; }
        public string GroupName { get; set; }
        public string TerminalName { get; set; }
        public SettingsGroup SettingsGroup { get; set; }
        public SettingsTerminal SettingsTerminal { get; set; }
        public AdminError AdminError { get; set; }
        public bool HasUsers
        {
            get
            {
                return Users.Any();
            }
        }
        public bool HasGroups
        {
            get
            {
                return Groups.Any();
            }
        }

        public AdminPageModel()
        {
            Users = new List<User>();
            Groups = new List<Group>();
            UserGroups = new List<UserGroupNamePair>();
            SettingsGroup = new SettingsGroup();
        }
    }
}
