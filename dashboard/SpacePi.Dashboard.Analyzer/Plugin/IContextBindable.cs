using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public interface IContextBindable {
        List<Binding<ContextClass>> ContextBindings { get; }
    }
}
