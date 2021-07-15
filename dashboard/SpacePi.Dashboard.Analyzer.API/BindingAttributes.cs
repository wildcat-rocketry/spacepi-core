using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    [AttributeUsage(AttributeTargets.Class)]
    public class BindingFactoryAttribute : Attribute {
        public BindingFactoryAttribute(Type declarativeAttribute, Type bindingAttribute) {
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class BindingFactoryArrayAttribute : Attribute {
        public BindingFactoryArrayAttribute(Type elementType) {
        }
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class BindingFactoryBoxerAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class BindingFactoryUnboxerAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingFactoryObjectAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Method)]
    public class BindingFactoryLoaderAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingIDAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingParameterAttribute : Attribute {
        public BindingParameterAttribute(string name) {
        }
    }

    [AttributeUsage(AttributeTargets.Parameter)]
    public class BindingPriorityAttribute : Attribute {
    }

    [AttributeUsage(AttributeTargets.Class)]
    public class EntryPointAttribute : Attribute {
    }
    
    [AttributeUsage(AttributeTargets.Assembly)]
    public class GenerateBindingsAttribute : Attribute {
    }
}
