using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.Analyzer {
    public class DebugLog : StringWriter {
        private static readonly string BaseDir = $"{BuildConfig.CMAKE_BINARY_DIR}/_dashboard/{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(Analyzer)}/{nameof(DebugLog)}";
        private readonly string FileName;
        private readonly Diagnostics Diagnostics;
        private readonly TextWriter StdOut;
        private readonly TextWriter StdErr;

        public void CatchAll(Action action) {
            try {
                action();
            } catch (Exception ex) {
                Console.Error.WriteLine(ex);
                Diagnostics.AnalyzerException.Report(ex.GetType().Name, ex.Message);
            }
        }

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
