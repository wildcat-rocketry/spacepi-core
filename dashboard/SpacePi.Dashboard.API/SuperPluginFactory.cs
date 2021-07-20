using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

namespace SpacePi.Dashboard.API {
    [BindingFactory(typeof(SuperPluginAttribute), typeof(BindSuperPluginAttribute)), EntryPoint]
    public class SuperPluginFactory : PluginFactory {
        [Subfactory]
        public IBoundFactory<PluginFactory> PluginFactory { get; set; }

        public override void Load() {
            base.Load();
            PluginFactory.Create();
        }
    }
}
