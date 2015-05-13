using System;
using System.Collections.Generic;

namespace Server.Models
{
    public class ViewGroupPrize
    {
        public Guid GroupId { get; set; }
        public string GroupName { get; set; }
        public bool Status { get; set; }
        public ushort ValuePrize { get; set; }
        public double Fund { get; set; }
    }
    
    public class PrizeFundPageModel
    {
        public List<ViewGroupPrize> Groups { get; set; }
        public PrizeFundPageModel()
        {
            Groups = new List<ViewGroupPrize>();
        }
    }
}
