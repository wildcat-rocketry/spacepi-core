using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class PrimitiveNode : ScalarNodeBase<IPrimitiveField>, IValueNode, IReloadable {
        private string _Value;
        private bool _Valid;

        public string Name => "Value";

        public event PropertyChangedEventHandler PropertyChanged;

        public static readonly PropertyChangedEventArgs ValueChanged = new(nameof(Value));
        public string Value {
            get => _Value;
            set {
                if (_Value != value) {
                    _Value = value;
                    if (Valid = IPrimitiveField.TryParsers[(int) Field.Type](value, out object v)) {
                        Field[Index] = v;
                    }
                    PropertyChanged?.Invoke(this, ValueChanged);
                }
            }
        }

        public static readonly PropertyChangedEventArgs ValidChanged = new(nameof(Valid));
        public bool Valid {
            get => _Valid;
            private set {
                if (_Valid != value) {
                    _Valid = value;
                    PropertyChanged?.Invoke(this, ValidChanged);
                }
            }
        }

        public override void Reload() {
            _Value = IPrimitiveField.ToStringers[(int) Field.Type](Field[Index]);
            _Valid = true;
            PropertyChanged?.Invoke(this, ValueChanged);
            PropertyChanged?.Invoke(this, ValidChanged);
        }

        public PrimitiveNode(IPrimitiveField field, int index = 0) : base(field, index) => Reload();
    }
}
