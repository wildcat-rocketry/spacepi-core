FROM debian:bullseye

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y git libgps-dev build-essential cmake mosquitto debootstrap qemu-user-static dosfstools kpartx sudo libboost-all-dev libprotobuf-dev default-jdk libssl-dev libgpiod-dev protobuf-compiler libi2c-dev
	
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

VOLUME ["/spacepi"]

#RUN mkdir -p /home/user ;\
#	mkdir -p /home/user/.ssh ;\
#	ssh-keyscan -t rsa github.com >> /home/user/.ssh/known_hosts ;\
#	git clone git@github.com:final-frontier-aerospace/space-pi.git ;\
#	mkdir space-pi/build ;\
#	cd space-pi/build ;\
#	cmake ..

CMD if [ -z $USER_ID ]; then USER_ID=1000; fi;\
	useradd -d /home/user -m -u $USER_ID user; \
	cd /spacepi ;\
	sleep infinity
	#su user -s /bin/bash
