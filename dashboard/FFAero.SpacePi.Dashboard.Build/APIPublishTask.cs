using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Build.Framework;

namespace FFAero.SpacePi.Dashboard.Build {
    public class APIPublishTask : BuildToolTask {
        [Required]
        public string BuildDir { get; set; }

        [Required]
        public string PackageId { get; set; }

        [Required]
        public string Version { get; set; }

        protected override string GenerateCommandLineCommands() {
            return $@"{nameof(APIPublishTask)} ""{BuildDir.Replace(@"\", @"/")}"" ""{PackageId}"" ""{Version}""";
        }
    }
}
