using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumOrdinalNode : ScalarNodeBase<IEnumField>, IValueNode, IReloadable {
        private string _Value;

        public string Name => "Ordinal Value";

        public string Value {
            get => _Value;
            set {
                _Value = value;
                if (Valid = int.TryParse(Value, out int v)) {
                    Field[Index] = v;
                }
            }
        }

        public bool Valid { get; private set; }

        public override void Reload() {
            _Value = Field[Index].ToString();
            Valid = true;
        }

        public EnumOrdinalNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
