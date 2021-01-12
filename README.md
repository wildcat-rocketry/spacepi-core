# Spacepi Target Docker

This is a docker target for testing SpacePi.

## Requirements

Currently this requires a Linux host with [docker](https://docs.docker.com/engine/install/) installed to run.
WSL2 will also work.

## Usage

To build the docker image use:
```
./spacepi-target-docker.sh build
```

After the build completes you'll want to start the container with:
```
./spacepi-target-docker.sh start
```

To open a root shell use:
```
./spacepi-target-docker.sh root-shell
```


After you're done using the container, close it with:
```
./spacepi-target-docker.sh stop
```
