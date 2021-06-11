using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.Core.DeveloperTools {
    abstract class ScalarNodeBase<TField> : IReloadable where TField : IField {
        public TField Field { get; private set; }

        public int Index { get; private set; }

        public abstract void Reload();

        public void FullReload(TField field, int index = 0) {
            Field = field;
            Index = index;
            Reload();
        }

        public ScalarNodeBase(TField field, int index) => FullReload(field, index);
    }
}
