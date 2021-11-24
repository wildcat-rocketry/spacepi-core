using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.Analyzer.API;

[assembly: CompileProtobuf(
    new[] {
        $"{BuildConfig.CMAKE_CURRENT_SOURCE_DIR}/../../protoc/csharp/include"
    },
    new[] {
        "google/protobuf/descriptor.proto",
        "google/protobuf/struct.proto"
    }
)]
