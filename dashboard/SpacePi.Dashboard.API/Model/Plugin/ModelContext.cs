using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Plugin {
    public class ModelContext : IContext {
        public void Dispose() => GC.SuppressFinalize(this);

        internal ModelContext(ModelPlugin plugin) {
        }
    }
}
