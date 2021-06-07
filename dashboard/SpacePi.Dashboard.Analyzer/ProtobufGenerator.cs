using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    [Generator]
    public class ProtobufGenerator : ISourceGenerator {
        public static readonly string BaseOutputDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.Analyzer/protobuf/";
        public static readonly string BuildLogFilename = "stdout.log";
        public static readonly string ErrorLogFilename = "stderr.log";
        public static readonly string StampFileName = ".stamp";
        public static readonly string PluginBuildLog = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.Analyzer/protobuf/{BuildLogFilename}";
        public static readonly string PluginErrorLog = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/SpacePi.Dashboard.Analyzer/protobuf/{ErrorLogFilename}";

        public void Execute(GeneratorExecutionContext context) {
            string[] files = context.AdditionalFiles
                .Select(f => f.Path)
                .Where(f => f.EndsWith(".proto"))
                .ToArray();
            if (files.Length == 0) {
                return;
            }
            string includeDir = Path.GetFullPath(files.Aggregate((a, b) => a.Substring(0, a.Zip(b, (A, B) => A == B).TakeWhile(v => v).Count()))).Replace('\\', '/');
            string outputDir = Path.GetFullPath($"{BaseOutputDir}/{includeDir.Substring(BuildConfig.CMAKE_SOURCE_DIR.Length)}").Replace('\\', '/');
            if (NeedsSourceRebuild(files, outputDir)) {
                if (Directory.Exists(outputDir)) {
                    Directory.Delete(outputDir, true);
                }
                Directory.CreateDirectory(outputDir);
                RebuildPlugin(context);
                ProcessStartInfo psi = new() {
                    Arguments = $"-I \"{includeDir}\" \"--spacepi-csharp_out={outputDir}\"{string.Join("", files.Select(f => $" \"{f.Substring(includeDir.Length).Replace('\\', '/')}\""))}",
                    CreateNoWindow = true,
                    FileName = BuildConfig.Protobuf_PROTOC_EXECUTABLE,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false
                };
                psi.Environment["PATH"] = $"{Path.GetDirectoryName(BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE)}{Path.PathSeparator}{psi.Environment["PATH"]}";
                Process proc = Process.Start(psi);
                Task<string> stdout = proc.StandardOutput.ReadToEndAsync();
                Task<string> stderr = proc.StandardError.ReadToEndAsync();
                proc.WaitForExit();
                stdout.Wait();
                stderr.Wait();
                File.WriteAllText($"{outputDir}/{BuildLogFilename}", stdout.Result);
                File.WriteAllText($"{outputDir}/{ErrorLogFilename}", stderr.Result);
                if (proc.ExitCode == 0) {
                    File.WriteAllText($"{outputDir}/{StampFileName}", "");
                } else {
                    context.ReportDiagnostic(Diagnostic.Create(Diagnostics.ProtocFailure, null));
                }
            }
            AddSourcesRecursive(outputDir, context);
        }

        private static bool NeedsSourceRebuild(string[] files, string outputDir) {
            string stampFile = $"{outputDir}/{StampFileName}";
            if (!File.Exists(stampFile) || !File.Exists(BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE)) {
                return true;
            }
            DateTime stamp = File.GetLastWriteTimeUtc(stampFile);
            if (File.GetLastWriteTimeUtc(BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE) > stamp) {
                return true;
            }
            foreach (string file in files) {
                if (File.GetLastWriteTimeUtc(file) > stamp) {
                    return true;
                }
            }
            return false;
        }

        private static void RebuildPlugin(GeneratorExecutionContext context) {
            ProcessStartInfo psi = new() {
                Arguments = $"--build \"{BuildConfig.CMAKE_BINARY_DIR}\" --target {BuildConfig.protoc_gen_spacepi_csharp.TARGET_NAME_IF_EXISTS}",
                CreateNoWindow = true,
                FileName = BuildConfig.CMAKE_COMMAND,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false
            };
            Process proc = Process.Start(psi);
            Task<string> stdout = proc.StandardOutput.ReadToEndAsync();
            Task<string> stderr = proc.StandardError.ReadToEndAsync();
            proc.WaitForExit();
            stdout.Wait();
            stderr.Wait();
            File.WriteAllText(PluginBuildLog, stdout.Result);
            File.WriteAllText(PluginErrorLog, stderr.Result);
            if (proc.ExitCode != 0) {
                context.ReportDiagnostic(Diagnostic.Create(Diagnostics.ProtocBuildFailed, null));
            }
        }

        private static void AddSourcesRecursive(string dir, GeneratorExecutionContext context) {
            foreach (string subdir in Directory.GetDirectories(dir)) {
                AddSourcesRecursive(subdir, context);
            }
            foreach (string file in Directory.GetFiles(dir).Where(f => f.EndsWith(".cs"))) {
                context.AddSource(Path.GetFileNameWithoutExtension(file), File.ReadAllText(file));
            }
        }

        public void Initialize(GeneratorInitializationContext context) {
        }
    }
}
