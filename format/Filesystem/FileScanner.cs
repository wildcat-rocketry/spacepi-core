using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Extensions.FileSystemGlobbing;
using SpacePi.Format.Model;

namespace SpacePi.Format.Filesystem {
    public static class FileScanner {
        private static readonly string FormatFileName = ".spacepi-format";

        public static IEnumerable<FormatSet> FindFiles(string rootDir) {
            List<DirectoryNode> roots = new();
            Stack<DirectoryNode> nodes = new();
            nodes.Push(new(Path.GetFullPath(rootDir)));
            SingleDirectoryPath sdp = new();
            while (nodes.Count > 0) {
                DirectoryNode node = nodes.Pop();
                node.Format = node.ParentDirectory?.Format;
                foreach (string path in Directory.EnumerateFileSystemEntries(node.AbsolutePath)) {
                    string name = Path.GetFileName(path);
                    if (Directory.Exists(path)) {
                        node.Subdirectories.Add(name, new DirectoryNode(node, name));
                    } else {
                        node.Files.Add(name, new FileNode(node, name));
                        if (name == FormatFileName) {
                            DirectoryNode root = new(node.AbsolutePath);
                            DirectoryNode newNode = new(root);
                            newNode.Subdirectories = node.Subdirectories;
                            newNode.Files = node.Files;
                            node.ParentDirectory?.Subdirectories?.Remove(node.Name);
                            node = newNode;
                            node.Format = SpacePiFormat.Parse(path);
                            root.Format = node.Format;
                            roots.Add(root);
                        }
                    }
                }
                if (node.Format != null) {
                    List<string> excluded = new();
                    foreach (KeyValuePair<string, DirectoryNode> subdir in node.Subdirectories) {
                        FileNode dot = new(subdir.Value, ".");
                        subdir.Value.Files.Add(dot.Name, dot);
                        if (sdp.Load(dot).IsExcluded(node.Format.Exclusions)) {
                            excluded.Add(subdir.Key);
                        } else {
                            nodes.Push(subdir.Value);
                        }
                    }
                    foreach (string subdir in excluded) {
                        node.Subdirectories.Remove(subdir);
                    }
                    excluded.Clear();
                    foreach (KeyValuePair<string, FileNode> file in node.Files) {
                        if (sdp.Load(file.Value).IsExcluded(node.Format.Exclusions)) {
                            excluded.Add(file.Key);
                        }
                    }
                    foreach (string file in excluded) {
                        node.Files.Remove(file);
                    }
                } else {
                    foreach (DirectoryNode subdir in node.Subdirectories.Values) {
                        nodes.Push(subdir);
                    }
                }
            }
            foreach (DirectoryNode root in roots) {
                foreach (ToolConfig tool in root.Format.Tools) {
                    RelativeMatcher matcher = new();
                    matcher.AddIncludePatterns(tool.Includes);
                    yield return new() {
                        Tool = tool,
                        Files = matcher.Execute(root).Files.Select(f => Path.GetFullPath(f.Path.Substring(DirectoryNode.RootDirectory.Length + 1), root.AbsolutePath)).ToList()
                    };
                }
            }
        }
    }
}
