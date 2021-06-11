using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumNamedNode : ScalarNodeBase<IEnumField>, IValueNode, IReloadable {
        private string _Value;
        private bool _Valid;

        public string Name => "Named Value";

        public event PropertyChangedEventHandler PropertyChanged;

        public static readonly PropertyChangedEventArgs ValueChanged = new(nameof(Value));
        public string Value {
            get => _Value;
            set {
                if (_Value != value) {
                    _Value = value;
                    if (Valid = Field.Type.NamedToOrdinal(value, out int v)) {
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
            _Valid = Field.Type.OrdinalToNamed(Field[Index], out _Value);
            PropertyChanged?.Invoke(this, ValueChanged);
            PropertyChanged?.Invoke(this, ValidChanged);
        }

        public EnumNamedNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
