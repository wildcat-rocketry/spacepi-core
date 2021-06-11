using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class ScalarNode<TField> : ScalarNodeBase<TField>, INode where TField : IField {
        public string Name { get; private set; }

        public override void Reload() => Name = Field.IsList ? $"{Field.Name}[{Index}]" : Field.Name;

        public ScalarNode(TField field, int index) : base(field, index) {
        }
    }
}
