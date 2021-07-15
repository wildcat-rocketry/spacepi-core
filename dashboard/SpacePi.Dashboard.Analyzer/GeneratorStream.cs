﻿using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.CodeAnalysis;

namespace SpacePi.Dashboard.Analyzer {
    public class GeneratorStream {
        private readonly StringBuilder Source = new();
        private readonly StringBuilder Indent = new();
        private bool OptionalNewline = false;

        public void AddIndent() => Indent.Append("    ");

        public void RemoveIndent() {
            if (Indent.Length >= 4) {
                Source.Remove(Source.Length - 5, 4);
                Indent.Remove(Indent.Length - 4, 4);
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
                        Source.Append(Indent);
                        break;
                    case '{':
                        AddIndent();
                        break;
                    case '}':
                        RemoveIndent();
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

        public void AppendConstructorCall(string symbol, bool paramsOnNewline, IEnumerable<Action> args) {
            Append("new");
            if (symbol != null) {
                Append($" {symbol}");
            }
            AppendTuple(paramsOnNewline, args);
        }

        public void AppendConstructorCall(string symbol, bool paramsOnNewline, params Action[] args) => AppendConstructorCall(symbol, paramsOnNewline, (IEnumerable<Action>) args);

        public void AppendConstructorCall(ITypeSymbol symbol, bool paramsOnNewline, IEnumerable<Action> args) => AppendConstructorCall(symbol?.ToString(), paramsOnNewline, args);

        public void AppendConstructorCall(ITypeSymbol symbol, bool paramsOnNewline, params Action[] args) => AppendConstructorCall(symbol, paramsOnNewline, (IEnumerable<Action>) args);

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

        public void AppendAssignment(Action lhs, Action rhs) => AppendStatement(() => {
            lhs();
            Append(" = ");
            rhs();
        });

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
