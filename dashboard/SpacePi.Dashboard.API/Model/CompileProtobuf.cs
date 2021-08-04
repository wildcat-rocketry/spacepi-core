using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

#if false
[assembly: CompileProtobuf(
    "google/protobuf/descriptor.proto",
    "google/protobuf/struct.proto"
)]
#else
namespace SpacePi.Dashboard.API.Model {
    public class Widget {
    }
}
#endif
