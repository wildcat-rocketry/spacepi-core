Getting Started
===============

Platform
--------

This page expects that the host which is compiling the SpacePi code is Linux-based.
On a non-Linux-based machine, there are a few options available for development:

1. Use the Windows Subsystem for Linux: https://docs.microsoft.com/en-us/windows/wsl/install-win10
2. Install Linux in a virtual machine
3. Install Linux on another computer and remote into it

Installing Dependencies
-----------------------

All commands in this section are tested on Debian 10.
Other versions or operating systems may cause differing results/errors.

Minimal Installation
********************

.. code-block:: text

    # apt update
    # apt install cmake build-essential protobuf-compiler libboost-all-dev

Installation for Dashboard
**************************

.. code-block:: text

    # apt install openjdk-11-jdk-headless

Installation for API Documentation
**********************************

.. code-block:: text

    # apt install doxygen graphviz

Installation for Full Documentation
***********************************

.. code-block:: text

    # apt install doxygen graphviz python3

Environment Setup
-----------------

After cloning the repository from Git but before building the code, the environment must first be set up with CMake.

First, enter the directory which the Git repository was cloned into (this may not be :code:`spacepi-core`, depending on which repository is being built):

.. code-block:: text

    $ cd spacepi-core

Then, create a new folder to contain all of the build artifacts in:

.. code-block:: text

    spacepi-core$ mkdir -p build

And enter the newly created :code:`build` folder:

.. code-block:: text

    spacepi-core$ cd build

Finally, run CMake to configure the project:

.. code-block:: text

    spacepi-core/build$ cmake ..

Building the Code
-----------------

After configuring CMake once, each time the code is edited and the project needs to be rebuilt, only the :code:`make` command is needed.
However, make sure the :code:`make` command is run inside the :code:`build` folder (:code:`cd` to it if needed).

Documentation Outputs
*********************

The core project can create a few additional outputs besides just the built code.
These are:

* :code:`build/html/index.html`: API Documentation
* :code:`build/sphinx/html/index.html`: Full Documenation

If the project being built was not the :code:`spacepi-core`, these may instead be under :code:`build/core` instead of just :code:`build`.

The following can be used to build the documentations (which is not built by default unless the build root is :code:`spacepi-core`):

.. code-block:: text

    spacepi-core/build$ make docs

Building Core Examples
**********************

The following can be used to build the core example code (which is not built by default):

.. code-block:: text

    spacepi-core/build$ make examples

Opening Dashboard Project
-------------------------

The dashboard project can be imported into either Eclipse IDE or IntelliJ IDEA IDE.
The following commands must be run on the platform on which the IDE is running.
Therefore, if the code is being built on WSL but the IDE is running on Windows, these commands should be run on Windows and not WSL.

These commands all assume the :code:`spacepi-core` repository was cloned.
If a different repository was cloned, instead use :code:`spacepi-other/core`.

Eclipse IDE
***********

Windows commands:

.. code-block:: text

    spacepi-core\dashboard> .\gradlew.bat eclipse

Linux commands:

.. code-block:: text

    spacepi-core/dashboard$ ./gradlew eclipse

Then, import the project into Eclipse by going to :code:`File` > :code:`Import...`, then select :code:`General` > :code:`Existing Projects into Workspace`, then select the :code:`spacepi-core/dashboard` folder as the project root directory and import the project.
Now, the project can be built and run directly from Eclipse.

IDEA IDE
********

Windows commands:

.. code-block:: text

    spacepi-core\dashboard> .\gradlew.bat idea

Linux commands:

.. code-block:: text

    spacepi-core/dashboard$ ./gradlew idea

Then, import the project into IDEA and select the :code:`spacepi-core/dashboard` folder as the project root directory.
Now, the project can be built and run directly from IDEA.
