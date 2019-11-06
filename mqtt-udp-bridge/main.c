#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <spacepi.h>
#include <errno.h>
#include <signal.h>

// number of arguments passed into the program (excluding the program name)
#define ARG_COUNT 4

// args: udp_address udp_port mqtt_server_address mqtt_server_port

struct sockaddr_in udp_addr;

// callback for pubsub system
void conn_callback() {
	
	// TODO implement this
	
}

// callback for SIGINT
void sigint(int signal) {
	
	// cleanup pubsub library
	if (spacepi_pubsub_cleanup() < 0) {
		fprintf(stderr, "Failed to cleanup pubsub library, oh well... (%d)\n", errno);
		exit(1);
	}
	exit(0);
	
}

int main(int argc, char ** argv, char ** envp) {
	
	// sanity checking the argument count
	if (argc != ARG_COUNT + 1) {
		fprintf(stderr, "Invalid number of arguments!\n");
		exit(1);
	}
	
	// register SIGINT callback
	signal(SIGINT, sigint);
	
	// initialize pubsub library
	if(spacepi_pubsub_init() < 0) {
		fprintf(stderr, "Failed to init pubsub library: %d\n", errno);
		exit(1);
	}
	
	return 0;
	
}