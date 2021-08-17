using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ScalarClassNode : AutoScalarGroupNode<IClassField> {
        private event Action Reloader;

        public override void Reload() {
            base.Reload();
            Reloader?.Invoke();
        }

        public ScalarClassNode(IClassField field, int index = 0) : base(field, index, "Reload Object") {
            INode[] nodes = Field[Index].Fields.Select<IField, INode>((f, i) => {
                if (f.IsList) {
                    ListNode n = new(f);
                    Reloader += () => n.FullReload(f);
                    return n;
                } else if (f is IClassField cf) {
                    ScalarClassNode n = new(cf);
                    Reloader += () => n.FullReload(cf);
                    return n;
                } else if (f is IEnumField ef) {
                    ScalarEnumNode n = new(ef);
                    Reloader += () => n.FullReload(ef);
                    return n;
                } else if (f is IPrimitiveField pf) {
                    ScalarPrimitiveNode n = new(pf);
                    Reloader += () => n.FullReload(pf);
                    return n;
                } else {
                    throw new ArgumentException("Unknown field type");
                }
            }).ToArray();
            SubGroups = nodes.OfType<IGroupNode>().ToArray();
            Values = nodes.OfType<IValueNode>().ToArray();
        }
    }
}
