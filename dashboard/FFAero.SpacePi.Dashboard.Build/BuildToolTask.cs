using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace FFAero.SpacePi.Dashboard.Build {
    public abstract class BuildToolTask : ToolTask {
        protected override string ToolName => "FFAero.SpacePi.Dashboard.BuildTool";

        protected override string GenerateFullPathToTool() {
            return Path.GetFullPath(Path.Combine(Assembly.GetExecutingAssembly().Location, "..", "..", "..", "..", "FFAero.SpacePi.Dashboard.BuildTool", "bin", "net6.0", "FFAero.SpacePi.Dashboard.BuildTool.exe"));
        }

        protected override abstract string GenerateCommandLineCommands();

        protected override MessageImportance StandardOutputLoggingImportance => MessageImportance.Normal;

        protected override MessageImportance StandardErrorLoggingImportance => MessageImportance.High;
    }
}
