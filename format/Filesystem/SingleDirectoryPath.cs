using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing.Abstractions;
using SpacePi.Format.Model;

namespace SpacePi.Format.Filesystem {
    public class SingleDirectoryPath : DirectoryInfoBase {
        private DirectoryNode Node;
        private SingleDirectoryPath Child;
        private SingleDirectoryPath ParentAlloc;
        private SingleDirectoryPath Parent;
        private FileNode File;

        public override string Name => Node.Name;
        public override string FullName => Node.FullName;
        public override DirectoryInfoBase ParentDirectory { get; }
        public override IEnumerable<FileSystemInfoBase> EnumerateFileSystemInfos() => new FileSystemInfoBase[] { Child, File }.Where(f => f != null);
        public override DirectoryInfoBase GetDirectory(string path) => path == Child?.Name ? Child : null;
        public override FileInfoBase GetFile(string path) => path == File?.Name ? File : null;

        public bool IsExcluded(IEnumerable<ExclusionEntry> excludes) => !excludes.FirstOrDefault(e => e.Check(this))?.IsInverted ?? false;

        public SingleDirectoryPath Load(FileNode file) {
            File = file;
            SingleDirectoryPath path = this;
            DirectoryNode dir = file.ParentDirectory;
            while (true) {
                path.Node = dir;
                dir = dir.ParentDirectory;
                if (dir == null) {
                    return path;
                } else {
                    if (path.ParentAlloc == null) {
                        path.ParentAlloc = new() { Child = path };
                    }
                    path.Parent = path.ParentAlloc;
                    path = path.Parent;
                }
            }
        }
    }
}
