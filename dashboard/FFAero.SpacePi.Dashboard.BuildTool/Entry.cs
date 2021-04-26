using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace FFAero.SpacePi.Dashboard.BuildTool {
    public static class Entry {
        public static void Main(string[] args) {
            if (args.Length < 1) {
                Console.Error.WriteLine("Must specify command");
                Environment.Exit(1);
            }
            IBuildTool tool = Assembly.GetExecutingAssembly().GetTypes()
                .Select(t => (t, t.GetCustomAttribute<BuildToolAttribute>()))
                .Where(t => t.Item2?.Name == args[0])
                .Select(t => t.t.GetConstructor(Array.Empty<Type>()).Invoke(Array.Empty<object>()))
                .Cast<IBuildTool>()
                .First();
            IEnumerable<(PropertyInfo, ToolArgumentAttribute)> props = tool.GetType().GetProperties()
                .Select(t => (t, t.GetCustomAttribute<ToolArgumentAttribute>()))
                .Where(t => t.Item2 != null);
            if (args.Length <= props.Max(p => p.Item2.Index)) {
                Console.Error.WriteLine("Expected more arguments");
                Environment.Exit(1);
            }
            foreach ((PropertyInfo, ToolArgumentAttribute) prop in props) {
                prop.Item1.SetValue(tool, args[prop.Item2.Index]);
            }
            tool.Run();
        }
    }
}
