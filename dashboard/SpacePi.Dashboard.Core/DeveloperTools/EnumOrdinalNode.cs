using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumOrdinalNode : ScalarNodeBase<IEnumField>, IValueNode, IReloadable {
        private string _Value;
        private bool _Valid;

        public string Name => "Ordinal Value";

        public event PropertyChangedEventHandler PropertyChanged;

        public static readonly PropertyChangedEventArgs ValueChanged = new(nameof(Value));
        public string Value {
            get => _Value;
            set {
                if (_Value != value) {
                    _Value = value;
                    if (Valid = int.TryParse(Value, out int v)) {
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
            _Value = Field[Index].ToString();
            _Valid = true;
            PropertyChanged?.Invoke(this, ValueChanged);
            PropertyChanged?.Invoke(this, ValidChanged);
        }

        public EnumOrdinalNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
