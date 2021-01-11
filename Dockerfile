# I'll try using this image as a base bacuase it apparently has full systemd
FROM alehaa/debian-systemd

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y git libgps-dev build-essential cmake mosquitto debootstrap qemu-user-static dosfstools kpartx sudo libboost-all-dev libprotobuf-dev default-jdk libssl-dev libgpiod-dev protobuf-compiler libi2c-dev cmake 
	
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

RUN mkdir /opt/spacepi-target-docker
COPY * /opt/spacepi-target-docker/

RUN cd /opt/spacepi-target-docker ; \
	mkdir build ; \
	cd build ; \
	cmake .. ; \
	make install ; \
	cd / ;
	

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
CMD [ "/sbin/init" ]
