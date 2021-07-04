using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public interface IArrayBinding : IBinding {
        IEnumerable<IClassBindingTarget> Targets { get; }
    }
}
