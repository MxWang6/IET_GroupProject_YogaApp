using System.IO;

namespace YogaApp.Utils
{
    class CsvWriter
    {
        public static void addPoseNameToCsv(string poseName)
        {
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            path += "\\Files\\PoseSelected.csv";

            if (File.Exists(path))
            {
                using (StreamWriter writer = new StreamWriter(path, false))
                {
                    writer.WriteLine(poseName);
                }
            }
        }
    }
}
