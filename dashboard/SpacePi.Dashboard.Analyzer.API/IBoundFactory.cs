using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpacePi.Dashboard.Analyzer.API {
    /// <summary>
    /// Interface used to obtain a child factory instance from a factory
    /// </summary>
    /// <typeparam name="TFactory">The type of subfactory</typeparam>
    public interface IBoundFactory<TFactory> where TFactory : new() {
        /// <summary>
        /// Creates a new instance of the subfactory
        /// </summary>
        /// <returns>The subfactory instance</returns>
        TFactory Create();
    }
}
