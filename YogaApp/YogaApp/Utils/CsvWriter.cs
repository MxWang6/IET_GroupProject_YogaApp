using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YogaApp.Utils
{
    class CsvWriter
    {
        public static void addPoseNameToCsv(string poseName)
        {
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            path += "\\..\\Files\\PoseSelected.csv";
            Console.WriteLine(path);

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
