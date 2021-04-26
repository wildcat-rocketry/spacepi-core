using NuGet.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.BuildTool {
    class ConsoleLogger : LoggerBase {
        public override void Log(ILogMessage message) {
            switch (message.Level) {
                case LogLevel.Error:
                case LogLevel.Warning:
                    Console.Error.WriteLine(message.Message);
                    break;
                case LogLevel.Minimal:
                case LogLevel.Information:
                    Console.WriteLine(message.Message);
                    break;
                case LogLevel.Verbose:
                case LogLevel.Debug:
                    break;
                default:
                    LogError($"Unknown logging level {message.Level}");
                    break;
            }
        }

        public override Task LogAsync(ILogMessage message) {
            return Task.Run(() => Log(message));
        }
    }
}
