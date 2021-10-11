using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace SpacePi.Format.Model {
    public class ToolConfig {
        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("includes")]
        public List<string> Includes { get; set; }

        [JsonProperty("configFile")]
        public string ConfigFile { get; set; }

        public bool MergeFrom(ToolConfig other) {
            if (Name == other.Name && ConfigFile == other.ConfigFile) {
                Includes = Includes.Concat(other.Includes).Distinct().ToList();
                return true;
            }
            return false;
        }

        public void MakePathsAbsolute(string rootDir) => ConfigFile = Path.GetFullPath(ConfigFile, rootDir);

        public static void Merge(List<ToolConfig> to, List<ToolConfig> from) {
            List<ToolConfig> additions = new();
            foreach (ToolConfig fromTool in from) {
                bool found = false;
                foreach (ToolConfig toTool in to) {
                    if (toTool.MergeFrom(fromTool)) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    additions.Add(fromTool);
                }
            }
            to.AddRange(additions);
        }
    }
}
