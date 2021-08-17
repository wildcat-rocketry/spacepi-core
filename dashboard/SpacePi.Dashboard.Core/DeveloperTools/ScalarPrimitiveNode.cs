using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ScalarPrimitiveNode : AutoScalarGroupNode<IPrimitiveField> {
        public override void Reload() {
            base.Reload();
            Values?.Cast<PrimitiveNode>().First().FullReload(Field, Index);
        }

        public ScalarPrimitiveNode(IPrimitiveField field, int index = 0) : base(field, index, "Reload Value") {
            SubGroups = Enumerable.Empty<IGroupNode>();
            Values = new IValueNode[] {
                new PrimitiveNode(Field, Index)
            };
            Reload();
        }
    }
}
