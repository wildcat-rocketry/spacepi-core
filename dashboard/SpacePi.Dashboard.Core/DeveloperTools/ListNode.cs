using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ListNode : IGroupNode, IReloadable {
        private readonly IField Field;
        private readonly Func<int, IGroupNode> ChildCtor;
        private readonly Action<int, IGroupNode> ChildReloader;

        public string Name { get; private set; }

        public IEnumerable<IActionNode> Actions { get; }

        public IEnumerable<IGroupNode> SubGroups { get; private set; }

        public IEnumerable<IValueNode> Values => Array.Empty<IValueNode>();

        private void ReloadTo(int stop) {
            IGroupNode[] arr = (IGroupNode[])SubGroups;
            for (int i = 0; i < stop; ++i) {
                ChildReloader(i, arr[i]);
            }
        }

        public void Reload() => ReloadTo(((IGroupNode[]) SubGroups).Length);

        public void FullReload(IField field) {
            Name = field.Name;
            IGroupNode[] old = (IGroupNode[]) SubGroups;
            IGroupNode[] @new = old;
            if (field.Count != old.Length) {
                @new = new IGroupNode[field.Count];
                old.CopyTo(@new, 0);
                for (int i = old.Length; i < field.Count; ++i) {
                    @new[i] = ChildCtor(i);
                }
            }
            SubGroups = @new;
            ReloadTo(Math.Min(field.Count, old.Length));
        }

        public ListNode(IField field) {
            Field = field;
            if (field is IClassField cf) {
                ChildCtor = i => new ScalarClassNode(cf, i);
                ChildReloader = (i, n) => ((ScalarClassNode) n).FullReload(cf, i);
            } else if (field is IEnumField ef) {
                ChildCtor = i => new ScalarEnumNode(ef, i);
                ChildReloader = (i, n) => ((ScalarEnumNode) n).FullReload(ef, i);
            } else if (field is IPrimitiveField pf) {
                ChildCtor = i => new ScalarPrimitiveNode(pf, i);
                ChildReloader = (i, n) => ((ScalarPrimitiveNode) n).FullReload(pf, i);
            } else {
                throw new ArgumentException("Unknown field type");
            }
            Name = field.Name;
            Actions = new IActionNode[] {
                new ReloadNode("Reload List", this),
                new ListAppendNode(field)
            };
            SubGroups = Enumerable.Range(0, Field.Count).Select(ChildCtor).ToArray();
        }
    }
}
