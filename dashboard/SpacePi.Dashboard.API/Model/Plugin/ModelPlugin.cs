using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.API.Model.Plugin {
    [Plugin(int.MinValue + 1)]
    public class ModelPlugin : IPlugin<ModelContext> {
        public string Name => $"{nameof(SpacePi)}.{nameof(Dashboard)}.{nameof(API)}.{nameof(Model)}";

        public Version Version => Assembly.GetExecutingAssembly().GetName().Version;

        public ModelContext CreateContext() => new(this);

        public void Dispose() => GC.SuppressFinalize(this);
    }
}
