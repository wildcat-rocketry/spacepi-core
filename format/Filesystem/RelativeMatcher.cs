using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing;

namespace SpacePi.Format.Filesystem {
    public class RelativeMatcher : Matcher {
        public override Matcher AddInclude(string pattern) {
            if (pattern.StartsWith("/")) {
                return base.AddInclude($"{DirectoryNode.RootDirectory}{pattern}");
            } else if (pattern.StartsWith("**")) {
                return base.AddInclude($"{DirectoryNode.RootDirectory}/{pattern}");
            } else {
                return base.AddInclude($"{DirectoryNode.RootDirectory}/**/{pattern}");
            }
        }
    }
}
