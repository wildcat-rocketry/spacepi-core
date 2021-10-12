using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ScalarEnumNode : ScalarGroupNode<IEnumField> {
        private readonly EnumOrdinalNode Ordinal;
        private readonly EnumNamedNode Named;

        public override IEnumerable<IGroupNode> SubGroups => Enumerable.Empty<IGroupNode>();

        public override IEnumerable<IValueNode> Values { get; }

        public override void Reload() {
            base.Reload();
            Ordinal?.FullReload(Field, Index);
            Named?.FullReload(Field, Index);
        }

        public ScalarEnumNode(IEnumField field, int index = 0) : base(field, index, "Reload Value") {
            Ordinal = new(field, index);
            Named = new(field, index);
            Values = new IValueNode[] {
                Ordinal,
                Named
            };
            Reload();
        }
    }
}
