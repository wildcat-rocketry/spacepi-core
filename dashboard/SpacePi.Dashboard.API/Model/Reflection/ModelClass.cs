using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Reflection {
    public class ModelClass : IClass {
        public string Name { get; }

        public IEnumerable<IField> Fields { get; protected set; }

        public ModelClass(string name) => Name = name;
    }
}
