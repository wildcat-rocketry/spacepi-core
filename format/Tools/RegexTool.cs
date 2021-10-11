using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace SpacePi.Format.Tools {
    public class RegexTool : IFormatTool {
        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            List<(Regex, string)> regexes = new();
            foreach (string line in File.ReadAllLines(formatFile)) {
                if (line.Length > 3) {
                    string[] parts = line.Split(line[0]);
                    if (parts.Length == 4 && parts[0].Length == 0 && parts[1].Length > 0 && parts[3].Length == 0) {
                        regexes.Add((new(parts[1], RegexOptions.Compiled), Regex.Unescape(parts[2])));
                    }
                }
            }
            bool success = true;
            foreach (string file in codeFiles) {
                string orig = File.ReadAllText(file);
                string contents = regexes.Aggregate(orig, (str, r) => r.Item1.Replace(str, r.Item2));
                if (orig != contents) {
                    if (write) {
                        File.WriteAllText(file, contents);
                    } else {
                        Console.Error.WriteLine($"Regex rule changes content of file {file}");
                        success = false;
                    }
                }
            }
            return success;
        }

        protected virtual void Dispose(bool disposing) {
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
