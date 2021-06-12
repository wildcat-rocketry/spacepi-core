using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class ModelEnum : IEnum {
        private readonly Dictionary<int, string> Names;
        private readonly Dictionary<string, int> Ordinals;

        public string Name { get; }

        public IEnumerable<string> ValueNames => Ordinals.Keys;

        public bool NamedToOrdinal(string named, out int ordinal) => Ordinals.TryGetValue(named, out ordinal);

        public bool OrdinalToNamed(int ordinal, out string named) => Names.TryGetValue(ordinal, out named);

        public ModelEnum(string name, params (int Ordinal, string Name)[] names) {
            Name = name;
            Names = names.ToDictionary(t => t.Ordinal, t => t.Name);
            Ordinals = names.ToDictionary(t => t.Name, t => t.Ordinal);
        }
    }
}
