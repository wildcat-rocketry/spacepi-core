using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// A log which can be used for debugging the analyzer
    /// </summary>
    public class DebugLog : StringWriter {
        /// <summary>
        /// The directory in which to store logs
        /// </summary>
        private static readonly string BaseDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}/{nameof(DebugLog)}";
        /// <summary>
        /// The name of the file in which to store the log
        /// </summary>
        private readonly string FileName;
        /// <summary>
        /// The diagnostics which have been reported
        /// </summary>
        private readonly Diagnostics Diagnostics;
        /// <summary>
        /// The original stdout
        /// </summary>
        private readonly TextWriter StdOut;
        /// <summary>
        /// The original stderr
        /// </summary>
        private readonly TextWriter StdErr;

        /// <summary>
        /// Runs a function while catching all exceptions
        /// </summary>
        /// <param name="action">The action to run</param>
        public void CatchAll(Action action) {
            try {
                action();
            } catch (Exception ex) {
                Console.Error.WriteLine(ex);
                Diagnostics.AnalyzerException.Report(ex.GetType().Name, ex.Message);
            }
        }

        /// <summary>
        /// Resets stdout and stderr and writes any debug to the file
        /// </summary>
        /// <param name="disposing">If the managed state should be disposed</param>
        protected override void Dispose(bool disposing) {
            Console.SetOut(StdOut);
            Console.SetError(StdErr);
            string log = ToString();
            if (log.Length > 0) {
                Directory.CreateDirectory(BaseDir);
                File.AppendAllText($"{BaseDir}/{FileName}.log", $@"
--------------------------------------------------------------------------------
{log}");
            }
            base.Dispose(disposing);
        }

        /// <summary>
        /// Creates a new debug log and sets itself to stdout and stderr
        /// </summary>
        /// <param name="filename">The name of the file in which to store the log</param>
        /// <param name="diagnostics">The diagnostics which have been reported</param>
        public DebugLog(string filename, Diagnostics diagnostics) {
            FileName = filename;
            Diagnostics = diagnostics;
            StdOut = Console.Out;
            StdErr = Console.Error;
            Console.SetOut(this);
            Console.SetError(this);
        }
    }
}
