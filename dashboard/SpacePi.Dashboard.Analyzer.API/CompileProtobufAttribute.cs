using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    [AttributeUsage(AttributeTargets.Assembly)]
    public class CompileProtobufAttribute : Attribute {
        public CompileProtobufAttribute(params string[] systemFiles) {
        }
    }
}
