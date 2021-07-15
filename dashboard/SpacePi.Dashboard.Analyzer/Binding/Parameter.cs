using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Binding {
    public record Parameter {
        public enum Types {
            FactoryObject,
            ID,
            Parameter,
            Priority
        }

        public IParameterSymbol Symbol;
        public Types Type;
        public string Name;
    }
}
