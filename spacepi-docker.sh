#!/bin/bash

# Script to execute docker builds and common spacepi tasks

spacepi_dir=$(dirname "$(readlink -f $0)")
user_uuid=$(id -u)
rep=/spacepi

function build {
	old_pwd=$PWD
	cd $spacepi_dir/Docker
	old_image=$(docker images -q ffaero:spacepi)
	if [[ -n $old_image ]]; then docker rmi --no-prune $old_image; fi
	docker build -t ffaero:spacepi .
	cd $old_pwd
}

# Start with 4GB stack size instead of default
function start {
	docker run --privileged=true \
		--rm --name spacepi -d -it \
		-e USER_ID=$user_uuid \
		-e QEMU_RESERVED_VA=0x7f000000 \
		-e TERM=$TERM \
		-v $spacepi_dir:$rep \
		ffaero:spacepi
	sleep 1 # One second delay to setup user
}

function stop {
	docker kill spacepi
}

case $1 in
	build)
		build
		;;
	shell)
		start
		docker exec -it -u user -w $rep spacepi /bin/bash
		stop
		;;
	root-shell)
		start
		docker exec -it -u root -w $rep spacepi /bin/bash
		stop
		;;
	cmake)
		mkdir -p $spacepi_dir/build
		start
		docker exec -it -u user -w $rep/build spacepi cmake ..
		stop
		;;
	*)
		start
		docker exec -it -u user -w $rep/build spacepi "${@}"
		stop
		;;
esac
