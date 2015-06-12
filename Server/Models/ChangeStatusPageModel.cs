using Server.Data;

namespace Server.Models
{
    class ChangeStatusPageModel
    {
        public Group Group { get; set; }
        public Terminal Terminal { get; set; }
        public byte State { get; set; }

        public ChangeStatusPageModel()
        {
        }
    }
}
