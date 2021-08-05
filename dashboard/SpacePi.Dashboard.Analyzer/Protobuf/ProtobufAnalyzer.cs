//#define PROTOBUF_DEBUG

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.Text;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer.Protobuf {
    /// <summary>
    /// The logic to compile protobuf code
    /// </summary>
    public class ProtobufAnalyzer {
        /// <summary>
        /// The compilation context
        /// </summary>
        private readonly Context Context;
        /// <summary>
        /// A list of all configurations to build
        /// </summary>
        private readonly IEnumerable<BuildConfiguration> Configs;

        /// <summary>
        /// Parses diagnostic messages output from the protobuf compiler
        /// </summary>
        /// <param name="diag">The type of diagnostic to report</param>
        /// <param name="line">The line of output to parse</param>
        /// <param name="files">A list of possible source files which could have diagnostics reported</param>
        private void ParseDiagnostic(Diagnostics.Type diag, string line, IEnumerable<string> files) {
            string[] parts = line.Split(new[] { ':' }, 2);
            string file = files.FirstOrDefault(f => f.EndsWith(parts[0]));
            Location loc = null;
            if (file != null && parts.Length == 2) {
                int colNo = 1;
                line = parts[1];
                parts = parts[1].Split(new[] { ':' }, 2);
                if (int.TryParse(parts[0], out int lineNo) && parts.Length == 2) {
                    line = parts[1];
                    parts = parts[1].Split(new[] { ':' }, 2);
                    if (int.TryParse(parts[0], out colNo) && parts.Length == 2) {
                        line = parts[1];
                    } else {
                        colNo = 1;
                    }
                } else {
                    lineNo = 1;
                }
                LinePosition pos = new(lineNo - 1, colNo - 1);
                loc = Location.Create(file, new TextSpan(), new LinePositionSpan(pos, pos));
            }
            diag.Report(loc, line);
        }

        /// <summary>
        /// Spawns a child process and parses the diagnostics reported by it
        /// </summary>
        /// <param name="filename">The executable to spawn</param>
        /// <param name="args">The arguments to the executable</param>
        /// <param name="path">An addition to the PATH environmental variable</param>
        /// <param name="files">A list of possible source files which could have diagnostics reported</param>
        /// <param name="stdoutDiag">The type of diagnostic to report for stdout</param>
        /// <param name="stderrDiag">The type of diagnostic to report for stderr</param>
        /// <returns></returns>
        private bool SpawnProcess(string filename, string args, string path, IEnumerable<string> files, Diagnostics.Type stdoutDiag, Diagnostics.Type stderrDiag) {
#if PROTOBUF_DEBUG
            if (path != null) {
                Console.Write($"PATH={path}{Path.PathSeparator}$PATH ");
            }
            Console.WriteLine($"{filename} {args}");
#endif
            ProcessStartInfo psi = new() {
                Arguments = args,
                CreateNoWindow = true,
                FileName = filename,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false
            };
            if (path != null) {
                psi.Environment["PATH"] = $"{path}{Path.PathSeparator}{psi.Environment["PATH"]}";
            }
            Process proc = Process.Start(psi);
            Task<string> stdout = proc.StandardOutput.ReadToEndAsync();
            Task<string> stderr = proc.StandardError.ReadToEndAsync();
            proc.WaitForExit();
            stdout.Wait();
            stderr.Wait();
            foreach (string line in stdout.Result.Split('\n').Select(l => l.Trim()).Where(l => l.Length > 0)) {
                ParseDiagnostic(stdoutDiag, line, files);
            }
            foreach (string line in stderr.Result.Split('\n').Select(l => l.Trim()).Where(l => l.Length > 0)) {
                ParseDiagnostic(stderrDiag, line, files);
            }
            return proc.ExitCode == 0;
        }

        /// <summary>
        /// Builds the protoc plugin with CMake
        /// </summary>
        private void BuildPlugin() => SpawnProcess(
            BuildConfig.CMAKE_COMMAND,
            $"--build \"{BuildConfig.CMAKE_BINARY_DIR}\" --target {BuildConfig.protoc_gen_spacepi_csharp.TARGET_NAME_IF_EXISTS}",
            null,
            Enumerable.Empty<string>(),
            Context.Diagnostics.CMakeBuildStatus,
            Context.Diagnostics.CMakeBuildError);

        /// <summary>
        /// Determines if the protobuf files need to be rebuilt
        /// </summary>
        /// <param name="config">The configuration to check</param>
        /// <param name="paths">The source files which can be built</param>
        /// <returns>If it needs to be rebuilt</returns>
        private bool NeedsRebuild(BuildConfiguration config, IEnumerable<string> paths) {
            if (!File.Exists(config.StampFile)) {
                return true;
            }
            DateTime stamp = File.GetLastWriteTimeUtc(config.StampFile);
            return paths.Concat(new[] { BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE })
                .Any(p => !File.Exists(p) || File.GetLastWriteTimeUtc(p) > stamp);
        }

        /// <summary>
        /// Builds a configuration of protobuf files
        /// </summary>
        /// <param name="config">The configuration to check</param>
        /// <param name="paths">The source files to be rebuilt</param>
        private void Build(BuildConfiguration config, IEnumerable<string> paths) {
            Directory.CreateDirectory(config.OutputDir);
            if (SpawnProcess(
                BuildConfig.Protobuf_PROTOC_EXECUTABLE,
                $"-I \"{config.SourceDir}\" \"--spacepi-csharp_out={config.OutputDir}\"{string.Join("", paths.Select(f => f.Substring(config.SourceDir.Length + 1)).Concat(config.SystemFiles).Select(f => $" \"{f.Replace('\\', '/')}\""))}",
                Path.GetDirectoryName(BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE),
                paths,
                Context.Diagnostics.ProtocBuildStatus,
                Context.Diagnostics.ProtocBuildError)) {
                File.WriteAllText(config.StampFile, "");
            }
        }

        /// <summary>
        /// Compiles a list of protobuf files
        /// </summary>
        /// <param name="files">The protobuf files to compile</param>
        public void CompileFiles(IEnumerable<AdditionalText> files) {
            string[] paths = files.Select(f => f.Path).Where(f => f.EndsWith(".proto")).ToArray();
            if (paths.Length == 0) {
                return;
            }
            Dictionary<BuildConfiguration, List<string>> builds = Configs.ToDictionary(c => c, c => new List<string>());
            foreach (string path in paths) {
                foreach (KeyValuePair<BuildConfiguration, List<string>> config in builds) {
                    if (path.StartsWith(config.Key.SourceDir)) {
                        config.Value.Add(path);
                        break;
                    }
                }
            }
            bool hasBuiltPlugin = false;
            foreach (KeyValuePair<BuildConfiguration, List<string>> config in builds.Where(c => c.Value.Any())) {
                if (NeedsRebuild(config.Key, config.Value)) {
                    if (!hasBuiltPlugin) {
                        hasBuiltPlugin = true;
                        BuildPlugin();
                    }
                    if (!File.Exists(BuildConfig.protoc_gen_spacepi_csharp.TARGET_FILE)) {
                        break;
                    }
                    Build(config.Key, config.Value);
                }
            }
        }

        /// <summary>
        /// Cleans all configuration outputs
        /// </summary>
        public void Clean() {
            foreach (BuildConfiguration config in Configs) {
                if (Directory.Exists(config.OutputDir)) {
                    Directory.Delete(config.OutputDir, true);
                }
            }
        }

        /// <summary>
        /// Adds all sources recursively in an output directory
        /// </summary>
        /// <param name="dir">The directory to add</param>
        /// <param name="context">The generator context</param>
        private void AddSourcesRecursive(string dir, GeneratorExecutionContext context) {
            foreach (string subdir in Directory.GetDirectories(dir)) {
                AddSourcesRecursive(subdir, context);
            }
            foreach (string file in Directory.GetFiles(dir).Where(f => f.EndsWith(".cs"))) {
                context.AddSource(Path.GetFileNameWithoutExtension(file), File.ReadAllText(file));
            }
        }

        /// <summary>
        /// Generates all sources
        /// </summary>
        /// <param name="context">The generator context</param>
        public void GenerateSources(GeneratorExecutionContext context) {
            foreach (BuildConfiguration config in Configs) {
                if (Directory.Exists(config.OutputDir)) {
                    AddSourcesRecursive(config.OutputDir, context);
                }
            }
        }

        /// <summary>
        /// Creates a new ProtobufAnalyzer
        /// </summary>
        /// <param name="ctx">The compilation context</param>
        public ProtobufAnalyzer(Context ctx) {
            Context = ctx;
            INamedTypeSymbol CompileProtobufAttribute = ctx.GetType<CompileProtobufAttribute>();
            Configs = ctx.Compilation.Assembly.GetAttributes()
                .Where(a => a.AttributeClass.DEquals(CompileProtobufAttribute))
                .Select(a => new BuildConfiguration(a))
                .OrderByDescending(c => c.SourceDir.Length);
#if PROTOBUF_DEBUG
            foreach (BuildConfiguration config in Configs) {
                Console.WriteLine(config);
            }
#endif
        }
    }
}
