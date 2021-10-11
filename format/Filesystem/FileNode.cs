using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing.Abstractions;
using SpacePi.Format.Model;

namespace SpacePi.Format.Filesystem {
    [SuppressMessage("Style", "IDE1006:Naming Styles", Justification = "Internal API")]
    public abstract class _FileNode : FileInfoBase {
        protected abstract DirectoryInfoBase _ParentDirectory { get; }
        public override DirectoryInfoBase ParentDirectory => _ParentDirectory;
    }

    public class FileNode : _FileNode {
        public override string Name { get; }

        public override string FullName { get; }

        public new DirectoryNode ParentDirectory { get; }
        protected override DirectoryInfoBase _ParentDirectory => ParentDirectory;

        public FileNode(DirectoryNode parent, string name) {
            Name = name;
            FullName = Path.Combine(parent.FullName, name);
            ParentDirectory = parent;
        }
    }
}
