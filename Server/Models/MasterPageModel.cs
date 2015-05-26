namespace Server.Models
{
    public class MasterPageModel
    {
        public string Title { get; set; }
        public string ProjectName { get; set; }
        public int Year { get; set; }
        public bool IsAuthenticated { get; set; }
        public string UserName { get; set; }
        public string Claim { get; set; }
        public bool IsAdmin { get; set; }
    }
}
