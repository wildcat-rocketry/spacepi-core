Getting Started
===============

Installing Dependencies
-----------------------

Standard Software Install
*************************

The following software packages are required to do SpacePi development:

1. Git: https://git-scm.com/
2. CMake: https://cmake.org/ (make sure to add to system PATH on Windows)
3. vcpkg: https://vcpkg.io/
4. Compiler Toolchain

Full Software Install
*********************

The following software packages are optional, but installing them enables more of SpacePi's functionality:

1. Visual Studio 2019 Preview 10+: https://visualstudio.microsoft.com/vs/preview/
2. .NET 6.0 Preview 3+: https://dotnet.microsoft.com/download/dotnet/6.0
3. Doxygen and Graphviz: https://www.doxygen.nl/
4. Python 3+: https://www.python.org/

Windows Subsystem for Linux
***************************

If building on a Windows machine, it may be desired to have a Linux environment as well to build in.
This can be achieved though the Windows Subsystem for Linux: https://docs.microsoft.com/en-us/windows/wsl/install-win10

Building the Code
-----------------

Build System Setup
******************

After cloning the repository from Git but before building the code, the environment must first be set up with CMake.

First, enter the directory which the Git repository was cloned into (this may not be :code:`spacepi-core`, depending on which repository is being built):

.. code-block:: text

    $ cd spacepi-core

Then, run CMake to configure the project:

.. code-block:: text

    $ cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=D:/github.com/Microsoft/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -DVCPKG_HOST_TRIPLET=x64-windows-static

This command contains many different options, which are summarized below:

* :code:`-S .`: Specifies that the source code is located in :code:`.` (aka the current directory)
* :code:`-B build`: Specifies that the built code goes in a new folder called :code:`build` (so :code:`spacepi-core/build`)
* :code:`-DCMAKE_TOOLCHAIN_FILE=.../vcpkg.cmake`: Specifies the location that vcpkg was installed into
* :code:`-DVCPKG_TARGET_TRIPLET=...`: Specifies the type of program to build
* :code:`-DVCPKG_HOST_TRIPLET=...`: Specifies the type of program which can run on this machine (likely the same as the previous)

The following triplets are supported and regularly tested:

* :code:`x64-windows-static`
* :code:`x64-linux`

The following triplets should be supported:

* :code:`*-windows-static`
* :code:`*-mingw-static`
* :code:`*-linux`
* :code:`*-osx`

Running a Build
***************

After configuring CMake once, each time the code is edited and the project needs to be rebuild, only the following commands are needed:

First, remember to navigate back to the directory the code is stored in:

.. code-block:: text

    $ cd spacepi-core

Then, run CMake to build the project:

.. code-block:: text

    $ cmake --build build

If the :code:`-B` option was changed when configuring the environment, the name of the build folder needs to be updated here as well.

Building with Make
******************

On a Linux system, :code:`make` can also be used to build the project, if it is installed.
First, navigate to the build folder:

.. code-block:: text

    $ cd spacepi-core/build

Then, run Make to build the project:

..code-block:: text

    $ make

Building with Visual Studio
***************************

If Microsoft Visual Studio is installed, it also can be used to build the project.

To build any C++ code, there should be a solution located in the build folder.
For example, if building the :code:`spacepi-core` repository, the solution file is located at :code:`spacepi-core/build/spacepi-core.sln`.

To build the C# dashboard application, there is a solution file located at :code:`spacepi-core/build/_dashboard/SpacePi Dashboard.sln`.
