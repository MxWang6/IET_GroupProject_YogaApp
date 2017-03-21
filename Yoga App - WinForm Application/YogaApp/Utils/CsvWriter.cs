using System;
using System.Collections.Generic;
using System.IO;

namespace YogaApp.Utils
{
    class CsvWriter
    {
        public static void addPoseNameToCsv()
        {
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            path += "\\Files\\PoseSelected.csv";
            List<String> lines = new List<String>();

            if (File.Exists(path))
            {
                using (StreamWriter writer = new StreamWriter(path, false))
                {
                    writer.WriteLine("pathak");
                }
            }
        }
    }
}
