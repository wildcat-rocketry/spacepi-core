using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace SpacePi.Format.Model {
    public class SpacePiFormat {
        [JsonProperty("include")]
        public List<string> Include { get; set; } = new();

        [JsonProperty("excludes")]
        public List<string> Excludes { get; set; } = new();

        [JsonProperty("excludeFiles")]
        public List<string> ExcludeFiles { get; set; } = new();

        [JsonProperty("tools")]
        public List<ToolConfig> Tools { get; set; } = new();

        public string RootDir { get; private set; }

        public ExclusionEntry[] Exclusions { get; private set; }

        public void MergeFrom(SpacePiFormat other) {
            Include.AddRange(other.Include);
            Excludes.AddRange(other.Excludes);
            ExcludeFiles.AddRange(other.ExcludeFiles);
            ToolConfig.Merge(Tools, other.Tools);
        }

        public void MakePathsAbsolute(string rootDir) {
            Include = Include.Select(f => Path.GetFullPath(f, rootDir)).ToList();
            ExcludeFiles = ExcludeFiles.Select(f => Path.GetFullPath(f, rootDir)).ToList();
            foreach (ToolConfig tool in Tools) {
                tool.MakePathsAbsolute(rootDir);
            }
            RootDir = rootDir;
        }

        public static SpacePiFormat Parse(string file) {
            SpacePiFormat node = new() { Include = { file } };
            node.MakePathsAbsolute(Path.GetDirectoryName(Path.GetFullPath(file)));
            while (node.Include.Count > 0) {
                string path = node.Include[0];
                node.Include.RemoveAt(0);
                SpacePiFormat child = JsonConvert.DeserializeObject<SpacePiFormat>(File.ReadAllText(path));
                child.MakePathsAbsolute(Path.GetDirectoryName(path));
                node.MergeFrom(child);
            }
            node.Exclusions = ExclusionEntry.Parse(node).Reverse().ToArray();
            return node;
        }
    }
}
