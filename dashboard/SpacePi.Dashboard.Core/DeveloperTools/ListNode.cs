using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class ListNode : IGroupNode, IReloadable {
        private readonly Func<int, IGroupNode> ChildCtor;
        private readonly Action<int, IGroupNode> ChildReloader;
        private IField Field;

        public event PropertyChangedEventHandler PropertyChanged;

        public string Name { get; }

        public IEnumerable<IActionNode> Actions { get; }

        public IEnumerable<IGroupNode> SubGroups { get; private set; }

        public IEnumerable<IValueNode> Values => Enumerable.Empty<IValueNode>();

        private void Reload(int from, int to) {
            IGroupNode[] old = (IGroupNode[]) SubGroups;
            if (Field.Count != old.Length) {
                IGroupNode[] @new = new IGroupNode[Field.Count];
                old.CopyTo(@new, 0);
                for (int i = old.Length; i < Field.Count; ++i) {
                    @new[i] = ChildCtor(i);
                }
                SubGroups = @new;
                PropertyChanged?.Invoke(this, IGroupNode.SubGroupsChanged);
                old = @new;
            }
            for (int i = from; i < to; ++i) {
                ChildReloader(i, old[i]);
            }
        }

        public void Reload() => Reload(0, ((IGroupNode[]) SubGroups).Length);

        public void FullReload(IField field) {
            Field = field;
            Reload();
        }

        private void ModelCollectionChanged(object sender, NotifyCollectionChangedEventArgs e) {
            int index = e.NewStartingIndex < 0 ? e.OldStartingIndex : e.NewStartingIndex;
            switch (e.Action) {
                case NotifyCollectionChangedAction.Add:
                case NotifyCollectionChangedAction.Remove:
                    Reload(index < 0 ? 0 : index, Field.Count);
                    break;
                case NotifyCollectionChangedAction.Replace:
                    if (index < 0) {
                        Reload();
                    } else {
                        Reload(index, index + e.NewItems.Count);
                    }
                    break;
                case NotifyCollectionChangedAction.Move:
                case NotifyCollectionChangedAction.Reset:
                default:
                    Reload();
                    break;
            }
        }

        public void Dispose() {
            foreach (IActionNode node in Actions) {
                node.Dispose();
            }
            foreach (IGroupNode node in SubGroups) {
                node.Dispose();
            }
            foreach (IValueNode node in Values) {
                node.Dispose();
            }
            Field.CollectionChanged -= ModelCollectionChanged;
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
            Field.CollectionChanged += ModelCollectionChanged;
        }
    }
}
