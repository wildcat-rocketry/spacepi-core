using SpacePi.Dashboard.API;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Core.StartupContext {
    [Plugin(int.MaxValue)]
    public class StartupContextPlugin : CorePlugin, IPlugin {
        protected override string PluginName => nameof(StartupContext);

        [BindPlugin]
        public IPluginFactory Factory { get; set; }

        public void Load() => Factory.CreateContext();
    }
}
