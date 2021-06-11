using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class PrimitiveNode : ScalarNodeBase<IPrimitiveField>, IValueNode, IReloadable {
        private string _Value;

        public string Name => "Value";

        public string Value {
            get => _Value;
            set {
                _Value = value;
                if (Valid = IPrimitiveField.TryParsers[(int) Field.Type](value, out object v)) {
                    Field[Index] = v;
                }
            }
        }

        public bool Valid { get; private set; }

        public override void Reload() {
            _Value = IPrimitiveField.ToStringers[(int) Field.Type](Field[Index]);
            Valid = true;
        }

        public PrimitiveNode(IPrimitiveField field, int index = 0) : base(field, index) => Reload();
    }
}
