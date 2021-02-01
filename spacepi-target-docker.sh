#!/bin/bash

# Script to execute docker builds and common spacepi tasks

script_dir=$(dirname "$(readlink -f $0)")

function get_git_top(){
	if [ -n "$1" ]; then
		rel_dir="$1"
	else
		rel_dir="."
	fi

	git_dir=$(git -C $rel_dir rev-parse --git-dir)

	until [ "$git_dir" == ".git" ]; do
		rel_dir="${git_dir}/.."
		git_dir=$(git -C $rel_dir rev-parse --git-dir)
		if [ $? != 0 ]; then
			exit 1
		fi
	done

	echo $(realpath "$rel_dir")
}

spacepi_dir=$(get_git_top $script_dir)
user_uuid=$(id -u)
rep=/spacepi

function build {
	old_pwd=$PWD
	cd $script_dir
	old_image=$(docker images -q ffaero:spacepi-target-docker)
	if [[ -n $old_image ]]; then docker rmi --no-prune $old_image; fi
	docker build -t ffaero:spacepi-target-docker .
	cd $old_pwd
}

# Start with 4GB stack size instead of default
function start {
	docker run --cap-add SYS_ADMIN \
		--name spacepi-target-docker -d -it \
		-e USER_ID=$user_uuid \
		-e QEMU_RESERVED_VA=0x7f000000 \
		-e TERM=$TERM \
		-v $spacepi_dir:$rep \
		-v /sys/fs/cgroup:/sys/fs/cgroup:ro \
		ffaero:spacepi-target-docker
	sleep 1 # One second delay to setup user
}

function stop {
	docker stop spacepi-target-docker
	docker container rm spacepi-target-docker
}

case $1 in
	build)
		build
		;;
	shell)
		docker exec -it -u user -w $rep spacepi-target-docker /bin/bash
		;;
	start)
		start
		;;
	stop)
		stop
		;;
	root-shell)
		docker exec -it -u root -w $rep spacepi-target-docker /bin/bash
		;;
	cmake)
		mkdir -p $spacepi_dir/build
		docker exec -it -u user -w $rep/build spacepi-target-docker cmake ..
		;;
	debug)
		docker run --cap-add SYS_ADMIN \
			--name spacepi-target-docker -d -it \
			-e USER_ID=$user_uuid \
			-e QEMU_RESERVED_VA=0x7f000000 \
			-e TERM=$TERM \
			-v $spacepi_dir:$rep \
			-v /sys/fs/cgroup:/sys/fs/cgroup:ro \
			--entrypoint /bin/bash \
			ffaero:spacepi-target-docker
		;;
	kill)
		docker kill spacepi-target-docker
		;;
	*)
		docker exec -it -u user -w $rep/build spacepi-target-docker "${@}"
		;;
esac
