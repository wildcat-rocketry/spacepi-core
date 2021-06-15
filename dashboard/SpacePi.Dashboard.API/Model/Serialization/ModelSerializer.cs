using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpacePi.Dashboard.API.Model.Reflection;

namespace SpacePi.Dashboard.API.Model.Serialization {
    public static class ModelSerializer {
        public static void Serialize(this IObject obj, Stream stream) => throw new NotImplementedException();

        public static void Parse(this IObject obj, Stream stream) => throw new NotImplementedException();
    }
}
