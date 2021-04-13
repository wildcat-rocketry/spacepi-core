using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace FFAero.SpacePi.Dashboard.API {
    public static class PluginFactory {
        public static readonly IEnumerable<IPlugin> LoadedPlugins;

        public static readonly IEnumerable<object> GlobalSettings;

        static PluginFactory() {
            LoadedPlugins = AppDomain.CurrentDomain.GetAssemblies()
                .SelectMany(a => a.GetTypes())
                .Where(t => t.GetCustomAttributes(typeof(PluginAttribute), false).Length > 0
                        && (t.Attributes & (TypeAttributes.Public | TypeAttributes.Class)) == (TypeAttributes.Public | TypeAttributes.Class)
                        && (t.Attributes & TypeAttributes.Abstract) == 0
                        && t.IsAssignableTo(typeof(IPlugin)))
                .Select(t => t.GetConstructor(Array.Empty<Type>()))
                .Where(c => c != null
                        && (c.Attributes & MethodAttributes.Public) != 0)
                .Select(c => c.Invoke(Array.Empty<object>()))
                .Cast<IPlugin>()
                .ToImmutableList();
            List<object> globalSettings = new();
            foreach (IPlugin plugin in LoadedPlugins) {
                // TODO
                plugin.RegisterSettings();
            }
            foreach (IPlugin plugin in LoadedPlugins) {
                plugin.Load();
            }
        }
    }
}
