using Server.Data;
using System.Collections.Generic;

namespace Server.Models
{
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
        public AdminPageModel()
        {
            Users = new List<User>();
            Groups = new List<Group>();
            UserGroups = new List<UserGroupNamePair>();
            SettingsGroup = new SettingsGroup();
        }
    }
}
