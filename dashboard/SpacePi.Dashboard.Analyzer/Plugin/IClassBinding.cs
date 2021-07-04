using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public interface IClassBinding : IBinding {
        IClassBindingTarget Target { get; }
    }
}
