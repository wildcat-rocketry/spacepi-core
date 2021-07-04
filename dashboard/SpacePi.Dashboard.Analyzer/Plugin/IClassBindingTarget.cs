﻿using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer.Plugin {
    public interface IClassBindingTarget {
        ITypeSymbol Symbol { get; set; }
        int Index { get; }
    }
}
