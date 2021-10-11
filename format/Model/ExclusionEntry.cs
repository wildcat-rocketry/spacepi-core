using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing.Abstractions;
using SpacePi.Format.Filesystem;

namespace SpacePi.Format.Model {
    public class ExclusionEntry {
        public bool IsInverted { get; }
        public string Glob { get; }
        private readonly RelativeMatcher Matcher;

        public bool Check(DirectoryInfoBase directoryInfo) => Matcher.Execute(directoryInfo).HasMatches;

        public static IEnumerable<ExclusionEntry> Parse(SpacePiFormat format) {
            foreach (string line in format.ExcludeFiles.SelectMany(f => File.ReadAllLines(f)).Concat(format.Excludes).Select(l => l.Trim())) {
                if (line.Length > 0 && !line.StartsWith("#")) {
                    if (line.StartsWith("!")) {
                        yield return new ExclusionEntry(true, line.Substring(1));
                    } else if (line.StartsWith("\\")) {
                        yield return new ExclusionEntry(false, line.Substring(1));
                    } else {
                        yield return new ExclusionEntry(false, line);
                    }
                }
            }
        }

        public ExclusionEntry(bool isInverted, string glob) {
            IsInverted = isInverted;
            Glob = glob;
            Matcher = new();
            Matcher.AddInclude(glob);
        }
    }
}
