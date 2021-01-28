# I'll try using this image as a base bacuase it apparently has full systemd
FROM alehaa/debian-systemd:buster

# Add Backports
RUN echo 'deb http://deb.debian.org/debian buster-backports main' >> /etc/apt/sources.list

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y autoconf-archive git libgps-dev build-essential mosquitto debootstrap qemu-user-static dosfstools kpartx sudo libboost-all-dev libprotobuf-dev default-jdk libssl-dev protobuf-compiler libi2c-dev && \
	apt-get -t buster-backports install -y cmake
	
RUN bash -c "echo -e \"ALL ALL=(ALL) NOPASSWD:ALL\\nDefaults env_keep += \\\"QEMU_RESERVED_VA\\\"\" > /etc/sudoers"

RUN cd /opt ; \
    git clone https://github.com/eclipse/paho.mqtt.c.git; \
	mkdir paho.mqtt.c/build; \
	cd paho.mqtt.c/build; \
	cmake ..; \
	make ;\
	make install ;\
	ldconfig ;\
	cd /;\
	rm -dRf /opt/paho.mqtt.c

RUN cd /opt ; \
	git clone https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git ; \
	cd libgpiod ; \
	git checkout v1.4 ; \
	mkdir build ; cd build ; \
	../autogen.sh --enable-bindings-cxx ; \
	make ; \
	make install ; \
	ldconfig ; \
	cd /; \
	rm -dRf /opt/libgpiod

RUN mkdir /opt/spacepi-target-docker
COPY . /opt/spacepi-target-docker/

RUN cd /opt/spacepi-target-docker ; \
	mkdir build ; \
	cd build ; \
	cmake .. ; \
	make install ; \
	ldconfig ; \
	cd / ; \
	mkdir -p /etc/spacepi ; \
	ln -s /opt/spacepi-target-docker/GroundStation.xml /etc/spacepi/new_conf.xml
	

# The host's cgroup filesystem need's to be mounted (read-only) in the
# container. '/run', '/run/lock' and '/tmp' need to be tmpfs filesystems when
# running the container without 'CAP_SYS_ADMIN'.
#
# NOTE: For running Debian stretch, 'CAP_SYS_ADMIN' still needs to be added, as
#       stretch's version of systemd is not recent enough. Buster will run just
#       fine without 'CAP_SYS_ADMIN'.
VOLUME [ "/spacepi/", "/sys/fs/cgroup", "/run", "/run/lock", "/tmp" ]

# As this image should run systemd, the default command will be changed to start
# the init system. CMD will be preferred in favor of ENTRYPOINT, so one may
# override it when creating the container to e.g. to run a bash console instead.
CMD [ "/usr/local/bin/spacepictl" ]
