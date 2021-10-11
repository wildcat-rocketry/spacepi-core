using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    /// <summary>
    /// A stream to generate code for generates sources
    /// </summary>
    public class GeneratorStream {
        /// <summary>
        /// The code string builder
        /// </summary>
        private readonly StringBuilder Source = new();
        /// <summary>
        /// The indent to use on each new line
        /// </summary>
        private readonly StringBuilder Indent = new();
        /// <summary>
        /// The number of '{'s on the current line
        /// </summary>
        private int LineBraceCount = 0;
        /// <summary>
        /// If a newline should be appended given that the next line is not a '}'
        /// </summary>
        private bool OptionalNewline = false;

        public void AddIndent(int tabs = 1) => Indent.Append(new string(' ', tabs * 4));

        public void RemoveIndent(int tabs = 1) {
            int spaces = tabs * 4;
            if (Indent.Length >= spaces) {
                Source.Remove(Source.Length - spaces - 1, spaces);
                Indent.Remove(Indent.Length - spaces, spaces);
            }
        }

        public void Append(IEnumerable<char> chars) {
            foreach (char c in chars) {
                if (OptionalNewline) {
                    OptionalNewline = false;
                    if (c != '}') {
                        Append("\n");
                    }
                }
                Source.Append(c);
                switch (c) {
                    case '\n':
                        if (LineBraceCount > 0) {
                            AddIndent(LineBraceCount);
                        }
                        LineBraceCount = 0;
                        Source.Append(Indent);
                        break;
                    case '{':
                        ++LineBraceCount;
                        break;
                    case '}':
                        if (--LineBraceCount < 0) {
                            LineBraceCount = 0;
                            RemoveIndent();
                        }
                        break;
                }
            }
        }

        public void AppendOptionalNewline() => OptionalNewline = true;

        public void Append(string text) => Append((IEnumerable<char>) text);

        public void AppendNamespace(string fqnn, Action contents) {
            Append($"namespace {fqnn} {{\n");
            contents();
            Append("}\n");
        }

        public void AppendClass(string name, Action contents, IEnumerable<ITypeSymbol> inherits) {
            Append($"public sealed class {name} ");
            bool first = true;
            foreach (ITypeSymbol type in inherits) {
                if (first) {
                    first = false;
                    Append(": ");
                } else {
                    Append(", ");
                }
                Append($"{type} ");
            }
            Append("{\n");
            contents();
            Append("}\n");
        }

        public void AppendClass(string name, Action contents, params ITypeSymbol[] inherits) => AppendClass(name, contents, (IEnumerable<ITypeSymbol>) inherits);

        public void AppendMethod(string access, string ret, string name, Action contents) {
            Append($"{access} {ret} {name}() {{\n");
            contents();
            Append("}\n");
            AppendOptionalNewline();
        }

        public void AppendMethod(string access, ITypeSymbol ret, string name, Action contents) => AppendMethod(access, ret.ToString(), name, contents);

        public void AppendPropertyDef(string access, string type, Action name) {
            Append($"{access} {type} ");
            name();
            Append(" { get; set; }\n");
            AppendOptionalNewline();
        }

        public void AppendPropertyDef(string access, ITypeSymbol type, Action name) => AppendPropertyDef(access, type.ToString(), name);

        public void AppendCast(Action source, ITypeSymbol target) {
            Append($"(({target}) ");
            source();
            Append(")");
        }

        public void AppendProperty(Action instance, IPropertySymbol prop) {
            AppendCast(instance, prop.ContainingType);
            Append($".{prop.Name}");
        }

        public void AppendArrayIndex(Action array, int index) {
            array();
            Append($"[{index}]");
        }

        public void AppendTuple(bool paramsOnNewline, IEnumerable<Action> vals) {
            Append("(");
            if (paramsOnNewline) {
                AddIndent();
                Append("\n");
            }
            bool first = true;
            foreach (Action val in vals) {
                if (first) {
                    first = false;
                } else if (paramsOnNewline) {
                    Append(",\n");
                } else {
                    Append(", ");
                }
                val();
            }
            if (paramsOnNewline) {
                Append("\n");
                RemoveIndent();
            }
            Append(")");
        }

        public void AppendTuple(bool paramsOnNewline, params Action[] vals) => AppendTuple(paramsOnNewline, (IEnumerable<Action>) vals);

        public void AppendMethodCall(Action instance, string method, IEnumerable<Action> args) {
            instance();
            Append($".{method}");
            AppendTuple(false, args);
        }

        public void AppendMethodCall(Action instance, string method, params Action[] args) => AppendMethodCall(instance, method, (IEnumerable<Action>) args);

        public void AppendMethodCall(Action instance, IMethodSymbol method, IEnumerable<Action> args) => AppendMethodCall(() => AppendCast(instance, method.ContainingType), method.Name, args);

        public void AppendMethodCall(Action instance, IMethodSymbol method, params Action[] args) => AppendMethodCall(instance, method, (IEnumerable<Action>) args);

        public void AppendConstructorCall(string symbol, bool paramsOnNewline, IEnumerable<Action> args, IEnumerable<Action> initProps) {
            Append("new");
            if (symbol != null) {
                Append($" {symbol}");
            }
            if (args != null || initProps == null) {
                AppendTuple(paramsOnNewline, args);
            }
            if (initProps != null) {
                Append(" {\n");
                foreach (Action initProp in initProps) {
                    initProp();
                    Append(",\n");
                }
                Append("}");
            }
        }

        public void AppendConstructorCall(string symbol, bool paramsOnNewline, params Action[] args) => AppendConstructorCall(symbol, paramsOnNewline, args, null);

        public void AppendConstructorCall(ITypeSymbol symbol, bool paramsOnNewline, IEnumerable<Action> args, IEnumerable<Action> initProps) => AppendConstructorCall(symbol?.ToString(), paramsOnNewline, args, initProps);

        public void AppendConstructorCall(ITypeSymbol symbol, bool paramsOnNewline, params Action[] args) => AppendConstructorCall(symbol, paramsOnNewline, args, null);

        public void AppendNewArray(ITypeSymbol type, IEnumerable<Action> elements) {
            Append($"new {type}[] {{\n");
            foreach (Action element in elements) {
                element();
                Append(",\n");
            }
            Append("}");
        }

        public void AppendNewVariable(ITypeSymbol type, Action name) {
            Append($"{type} ");
            name();
        }

        public void AppendNewArray(ITypeSymbol type, params Action[] elements) => AppendNewArray(type, (IEnumerable<Action>) elements);

        public void AppendStatement(Action contents) {
            contents();
            Append(";\n");
        }

        public void AppendAssignmentExpression(Action lhs, Action rhs) {
            lhs();
            Append(" = ");
            rhs();
        }

        public void AppendAssignment(Action lhs, Action rhs) => AppendStatement(() => AppendAssignmentExpression(lhs, rhs));

        public void AppendReturn(Action val) => AppendStatement(() => {
            Append("return ");
            val();
        });

        public void Clear() {
            Source.Clear();
            Indent.Clear();
        }

        public override string ToString() => Source.ToString();
    }
}
