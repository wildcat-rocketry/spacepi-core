using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class ScalarGroupNode<TField> : ScalarNode<TField>, IGroupNode where TField : IField {
        public IEnumerable<IActionNode> Actions { get; }

        public abstract IEnumerable<IGroupNode> SubGroups { get; }

        public abstract IEnumerable<IValueNode> Values { get; }

        public ScalarGroupNode(TField field, int index, string reloadName) : base(field, index) {
            Actions = Field.IsList ? new IActionNode[] {
                new ReloadNode(reloadName, this),
                new ListRemoveNode(Field, Index)
            } : new IActionNode[] {
                new ReloadNode(reloadName, this)
            };
        }
    }
}
