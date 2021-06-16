using System;
using System.Collections.Generic;
using System.Text;

namespace SpacePi.Dashboard.Analyzer.Plugin.Model {
    public record DebuggableRecord {
        protected virtual bool PrintMembers(StringBuilder str) => false;

        public bool Print(StringBuilder str) {
            str.Append("{ ");
            if (PrintMembers(str)) {
                str.Append(" ");
            }
            str.Append("}");
            return true;
        }

        protected bool PrintList<Type>(StringBuilder str, IEnumerable<Type> list) {
            str.Append("[ ");
            bool start = true;
            Type last = default;
            foreach (Type obj in list) {
                if (start) {
                    last = obj;
                    start = false;
                    continue;
                }
                if (last is DebuggableRecord dr) {
                    dr.Print(str);
                    str.Append(", ");
                } else {
                    str.Append($"\"{last}\", ");
                }
            }
            if (!start) {
                if (last is DebuggableRecord dr) {
                    if (dr.Print(str)) {
                        str.Append(" ");
                    }
                } else {
                    str.Append($"\"{last}\" ");
                }
            }
            str.Append("]");
            return true;
        }
    }
}
