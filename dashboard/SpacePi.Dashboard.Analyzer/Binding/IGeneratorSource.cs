using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public interface IGeneratorSource {
        GeneratedClass GeneratedClass { get; }
    }
}
