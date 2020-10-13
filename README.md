## Spacepi Target RPI

Here is a target for various architectures of Raspberry Pi devices. This shouldn't require significant changes in the build process for other arm boards.

### Building OS

The process of building the base OS images takes a long time and downloads a lot of packages from the Debian apt repo.
Therefore, it is recommended you run this process when you do not need your computer for a while.

Unlike when building modules, the `-j` option does not actually work for this process.
Even if specified, the process will still run on one core.
Exception: if targeting multiple architectures, it may build each architecture on its own core, but the output will be too confusing for any debugging to occur.

Note: this process does not currently work on WSL and has to be done on actual Linux, but WSL2 may work.

```
cd space-pi/build
make os
```

After you have built the OS, it will take up a lot of space on your drive.
If you wish to delete the OS images to save space (keep in mind it will take a long time to get them back), use the following commands:

```
cd space-pi/build
make os_clean
```

### Alternative Docker Build

Using docker, you don't have to pollute your install with build dependencies. And makes it nicer if you don't run a Debian based distro.

This still requires an OS with the Linux kernel for fuse mounts. Though untested, WSL2 ships with Linux kernel so may now support this build.

After getting docker installed, the os build is completed by executing:
```
./spacepi-docker.sh build
./spacepi-docker.sh cmake
./spacepi-docker.sh sudo make os
```

### Adding Users

Users and ssh keys are added by modifying the xml configuration loaded at `/etc/spacepi.xml`. That is explained somewhere else, the template in this repo can also be modified for use.
