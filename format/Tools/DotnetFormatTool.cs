using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis.Tools;

namespace SpacePi.Format.Tools {
    public class DotnetFormatTool : IFormatTool {
        private readonly MethodInfo MainMethod;

        public bool Format(IEnumerable<string> codeFiles, string formatFile, bool write) {
            if (!codeFiles.Any()) {
                return true;
            }
            string path = Path.GetDirectoryName(codeFiles.First());
            StringBuilder csproj = new StringBuilder();
            csproj.AppendLine(
@"<Project Sdk=""Microsoft.NET.Sdk"">
    <PropertyGroup>
        <EnableDefaultItems>false</EnableDefaultItems>
        <TargetFramework>net6.0</TargetFramework>
    </PropertyGroup>
    <ItemGroup>");
            foreach (string file in codeFiles.Skip(1)) {
                while (!file.StartsWith(path)) {
                    path = Path.GetDirectoryName(path);
                }
                csproj.AppendLine($@"        <Compile Include=""{file}"" />");
            }
            csproj.AppendLine(
@"    </ItemGroup>
</Project>");
            string proj = Path.Combine(path, ".spacepi-format.csproj");
            File.WriteAllText(proj, csproj.ToString());
            int r = (int) MainMethod.Invoke(null, new object[] { new[] { "-v", "diag", "style", "--no-restore", proj }.Concat(write ? Array.Empty<string>() : new[] { "--verify-no-changes" }).ToArray()});
            File.Delete(proj);
            return r == 0;
        }

        protected virtual void Dispose(bool disposing) {
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public DotnetFormatTool() => MainMethod = typeof(FormattedFile).Assembly.EntryPoint;
    }
}
