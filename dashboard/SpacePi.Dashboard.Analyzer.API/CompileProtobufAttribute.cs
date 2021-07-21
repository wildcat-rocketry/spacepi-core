using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// An attribute to mark a folder as a root of a protobuf compilation configuration
    /// </summary>
    [AttributeUsage(AttributeTargets.Assembly)]
    public class CompileProtobufAttribute : Attribute {
        /// <summary>
        /// Attribute constructor
        /// </summary>
        /// <param name="systemFiles">A list of system files to include in the compilation</param>
        public CompileProtobufAttribute(params string[] systemFiles) {
        }
    }
}
