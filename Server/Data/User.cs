using LinqToDB.Mapping;
using Nancy.Security;
using System;
using System.Collections.Generic;

namespace Server
{
    [Table("Users")]
    public class User : IUserIdentity
    {
        [PrimaryKey, Identity]
        public Guid Id { get; set; }
        [Column]
        public string UserName { get; set; }
        [Column]
        public string Password { get; set; }
        [Column]
        public string Claim { get; set; }

        public IEnumerable<string> Claims 
        {
            get 
            {
                IEnumerable<string> result = null;
                string claim = Claim;
                if (!string.IsNullOrEmpty(claim))
                {
                    result = new string[] { claim };
                }
                return result;
            } 
        }
    }
}
