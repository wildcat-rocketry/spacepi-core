Troubleshooting
===============

Common Issues
-------------

CMake Error at CMakeLists.txt:3 (include): include could not find load file: core/cmake/XXXXXXXXRepo.cmake
**********************************************************************************************************

Make sure the repository is recursively cloned.
To do this from command-line Git, use the following command:

.. code-block:: text

    spacepi-XXXXXXXX$ git submodule update --init --recursive

Alternatively, to avoid this problem when cloning the repository, use the following command:

.. code-block:: text

    $ git clone --recurse-submodules git@github.com:final-frontier-aerospace/spacepi-XXXXXXXX.git

E: Could not open lock file /var/lib/dpkg/lock-frontend
*******************************************************

Commands for installing software must be run as the :code:`root` user.
To run a single command as :code:`root` instead of the current user, use the :code:`sudo` command.

For example:

.. code-block:: text

    $ whoami
    zach
    $ sudo whoami
    root

/usr/bin/env: 'sh\\r': No such file or directory
************************************************

This is caused by a shell script with Windows line endings being run.
To fix the problem, add an entry for the shell script in the repository's :code:`.gitattributes` file similar to this:

.. code-block:: text

    /path/to/filename eol=lf

Then, delete the file and check it back out of Git to get the new line endings:

.. code-block:: text

    spacepi-XXXXXXXX$ rm path/to/filename
    spacepi-XXXXXXXX$ git checkout -- path/to/filename

make: \*\*\* No targets specified and no makefile found.  Stop.
***************************************************************

This is caused by not being inside a configured build folder.
Ensure that the current directory *is* in fact a build folder, and it has been configured with :code:`cmake` (without error).

[ERROR] [core] std::exception::what: bind: Permission denied
************************************************************

This error can be generated if the router cannot create a UNIX socket due to directory permissions.
To fix this, use the following command:

.. code-block:: text

    # chmod 1777 /run /var/run
