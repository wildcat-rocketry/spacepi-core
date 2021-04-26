using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NuGet.Commands;
using NuGet.Configuration;

namespace SpacePi.Dashboard.BuildTool {
    [BuildTool(nameof(APIPublishTask))]
    public class APIPublishTask : IBuildTool {
        [ToolArgument(1)]
        public string BuildDir { get; set; }

        [ToolArgument(2)]
        public string PackageId { get; set; }

        [ToolArgument(3)]
        public string Version { get; set; }

        public void Run() {
            string nugetRepo = Path.Combine(BuildDir, ".nuget");
            string packageDir = Path.Combine(nugetRepo, PackageId, Version);
            if (Directory.Exists(packageDir)) {
                Directory.Delete(packageDir, true);
            }
            ISettings settings = Settings.LoadDefaultSettings(BuildDir);
            string packageFile = Path.Combine(BuildDir, PackageId, "bin", $"{PackageId}.{Version}.nupkg");
            Task t = PushRunner.Run(
                settings,
                new PackageSourceProvider(settings),
                new[] { packageFile }.ToList(),
                nugetRepo,
                null,
                null,
                null,
                0,
                true,
                true,
                true,
                false,
                new ConsoleLogger());
            t.Wait();
            if (t.IsFaulted) {
                Environment.ExitCode = 1;
            }
        }
    }
}
