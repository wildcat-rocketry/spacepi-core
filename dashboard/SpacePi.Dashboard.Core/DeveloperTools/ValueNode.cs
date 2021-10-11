using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class ValueNode<TField> : ScalarNodeBase<TField>, IValueNode, IReloadable where TField : IField {
        private string _Value;
        private bool _Valid;

        public abstract string Name { get; }

        public event PropertyChangedEventHandler PropertyChanged;

        public string Value {
            get => _Value;
            set {
                if (_Value != value) {
                    _Value = value;
                    Valid = TryParse(value);
                    PropertyChanged?.Invoke(this, IValueNode.ValueChanged);
                }
            }
        }

        public bool Valid {
            get => _Valid;
            private set {
                if (_Valid != value) {
                    _Valid = value;
                    PropertyChanged?.Invoke(this, IValueNode.ValidChanged);
                }
            }
        }

        protected abstract bool TryParse(string str);

        protected abstract string ValueToString();

        public override void Reload() {
            _Value = ValueToString();
            _Valid = true;
            PropertyChanged?.Invoke(this, IValueNode.ValueChanged);
            PropertyChanged?.Invoke(this, IValueNode.ValidChanged);
        }

        private void ModelCollectionChanged(object sender, NotifyCollectionChangedEventArgs e) {
            int index = e.NewStartingIndex < 0 ? e.OldStartingIndex : e.NewStartingIndex;
            switch (e.Action) {
                case NotifyCollectionChangedAction.Add:
                case NotifyCollectionChangedAction.Remove:
                case NotifyCollectionChangedAction.Replace:
                    if (index <= Index) {
                        Reload();
                    }
                    break;
                case NotifyCollectionChangedAction.Move:
                    if (e.NewStartingIndex <= Index || e.OldStartingIndex <= Index) {
                        Reload();
                    }
                    break;
                case NotifyCollectionChangedAction.Reset:
                default:
                    Reload();
                    break;
            }
        }

        public void Dispose() => Field.CollectionChanged -= ModelCollectionChanged;

        public ValueNode(TField field, int index) : base(field, index) => Field.CollectionChanged += ModelCollectionChanged;
    }
}
