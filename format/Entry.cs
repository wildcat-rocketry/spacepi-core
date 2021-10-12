using System;
using System.Collections.Generic;
using System.CommandLine;
using System.CommandLine.Invocation;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Format.Filesystem;
using SpacePi.Format.Model;
using SpacePi.Format.Tools;

namespace SpacePi.Format {
    public static class Entry {
        public static Task<int> Main(string[] args) {
            RootCommand cmd = new() {
                new Argument<string>("directory", () => ".", "The root directory to format"),
                new Option<bool>(new[] { "--dry-run", "-n" }, "Do not write changes to files on disk"),
                new Option<bool>(new[] { "--debug", "-v" }, "Print debugging information about which files to format"),
            };
            cmd.Description = "spacepi-format";
            cmd.Handler = CommandHandler.Create(Run);
            return cmd.InvokeAsync(args);
        }

        public static async Task<int> Run(string directory, bool dryRun, bool debug) => await Task.Run(() => {
            List<FormatSet> jobs = FileScanner.FindFiles(directory).ToList();
            if (debug) {
                foreach (FormatSet job in jobs) {
                    Console.WriteLine($"Job with tool='{job.Tool.Name}' ({job.Tool.ConfigFile}):");
                    foreach (string file in job.Files) {
                        Console.WriteLine($"  - {file}");
                    }
                    Console.WriteLine();
                }
            }
            ToolDatabase tools = new();
            bool success = true;
            foreach (FormatSet job in jobs) {
                success = tools[job.Tool.Name].Format(job.Files, job.Tool.ConfigFile, !dryRun) && success;
            }
            return success ? 0 : 1;
        });
    }
}
