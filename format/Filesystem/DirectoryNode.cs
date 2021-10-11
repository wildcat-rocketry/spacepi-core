using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing.Abstractions;
using SpacePi.Format.Model;

namespace SpacePi.Format.Filesystem {
    [SuppressMessage("Style", "IDE1006:Naming Styles", Justification = "Internal API")]
    public abstract class _DirectoryNode : DirectoryInfoBase {
        protected abstract DirectoryInfoBase _ParentDirectory { get; }
        public override DirectoryInfoBase ParentDirectory => _ParentDirectory;

        protected abstract DirectoryInfoBase _GetDirectory(string path);
        public override DirectoryInfoBase GetDirectory(string path) => _GetDirectory(path);

        protected abstract FileInfoBase _GetFile(string path);
        public override FileInfoBase GetFile(string path) => _GetFile(path);
    }

    public class DirectoryNode : _DirectoryNode {
        public static readonly string RootDirectory = "__root__";

        public override string Name { get; }
        
        public override string FullName { get; }
        
        public new DirectoryNode ParentDirectory { get; }
        protected override DirectoryInfoBase _ParentDirectory => ParentDirectory;

        public Dictionary<string, DirectoryNode> Subdirectories { get; set; } = new();

        public Dictionary<string, FileNode> Files { get; set; } = new();

        public string AbsolutePath { get; }

        public SpacePiFormat Format { get; set; }

        public override IEnumerable<FileSystemInfoBase> EnumerateFileSystemInfos() => Subdirectories.Values.Cast<FileSystemInfoBase>().Concat(Files.Values.Cast<FileSystemInfoBase>());

        public new DirectoryNode GetDirectory(string path) {
            if (Subdirectories.TryGetValue(path, out DirectoryNode node)) {
                return node;
            }
            return null;
        }
        protected override DirectoryInfoBase _GetDirectory(string path) => GetDirectory(path);

        public new FileNode GetFile(string path) {
            if (Files.TryGetValue(path, out FileNode node)) {
                return node;
            }
            return null;
        }
        protected override FileInfoBase _GetFile(string path) => GetFile(path);

        public DirectoryNode(DirectoryNode parent, string name) {
            Name = name;
            FullName = $"{parent.FullName}/{name}";
            ParentDirectory = parent;
            AbsolutePath = Path.Combine(parent.AbsolutePath, name);
        }

        public DirectoryNode(DirectoryNode rootNode) {
            Name = RootDirectory;
            FullName = Name;
            ParentDirectory = rootNode;
            AbsolutePath = rootNode.AbsolutePath;
            rootNode.Subdirectories.Add(Name, this);
        }

        public DirectoryNode(string rootDir) {
            Name = RootDirectory;
            FullName = Name;
            ParentDirectory = null;
            AbsolutePath = rootDir;
        }
    }
}
