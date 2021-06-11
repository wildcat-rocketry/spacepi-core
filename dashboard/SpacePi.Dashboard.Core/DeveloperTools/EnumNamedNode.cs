using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    class EnumNamedNode : ScalarNodeBase<IEnumField>, IValueNode, IReloadable {
        private string _Value;

        public string Name => "Named Value";

        public string Value {
            get => _Value;
            set {
                _Value = value;
                if (Valid = Field.Type.NamedToOrdinal(value, out int v)) {
                    Field[Index] = v;
                }
            }
        }

        public bool Valid { get; private set; }

        public override void Reload() => Valid = Field.Type.OrdinalToNamed(Field[Index], out _Value);

        public EnumNamedNode(IEnumField field, int index) : base(field, index) {
        }
    }
}
