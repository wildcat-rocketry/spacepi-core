#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <spacepi.h>
#include <pthread.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>

// number of arguments passed into the program (excluding the program name)
#define ARG_COUNT 2
#define TOPIC_BUFFER_SIZE 0xFF
#define DATA_BUFFER_SIZE 0xFFFF

// args: udp_address udp_port

int udp_fd;
struct sockaddr_in udp_local_addr;
struct sockaddr_in udp_addr;
struct sockaddr_in udp_recv_addr;
char topic_buf[TOPIC_BUFFER_SIZE];
char data_buf[DATA_BUFFER_SIZE]; // buffer for recieving data over the network
pthread_t recv_thread;

int udp_recieve(char *, int);

// callback for pubsub system
void conn_callback() {
	
	// TODO implement this
	
}

void * recv_callback(void * arg) {
	while (1) {
		memset(&topic_buf, 0, TOPIC_BUFFER_SIZE);
		udp_recieve(topic_buf, TOPIC_BUFFER_SIZE - 1); // allow space for null character
		int datalen = udp_recieve(data_buf, DATA_BUFFER_SIZE);
		spacepi_publish(topic_buf, data_buf, datalen, sq_exactly_once, 0);
	}
}

void subscription_cb(void * context, const char * channel, const void * data, size_t data_len, spacepi_qos_t qos, int retain) {
	sendto(udp_fd, channel, strlen(channel), 0, (struct sockaddr *) &udp_addr, sizeof(udp_addr));
	sendto(udp_fd, data, data_len, 0, (struct sockaddr *) &udp_addr, sizeof(udp_addr));
}

// callback for SIGINT
void sigint(int signal) {
	
	// cleanup pubsub library
	if (spacepi_pubsub_cleanup() < 0) {
		fprintf(stderr, "Failed to cleanup pubsub library, oh well... (%s)\n", strerror(errno));
		exit(1);
	}
	exit(0);
	
}

int udp_recieve(char * buf, int len) {
	int size = sizeof(udp_recv_addr);
	int ret = recvfrom(udp_fd, buf, len, 0, (struct sockaddr *) &udp_recv_addr, &size);
	if (ret == len) {
		fprintf(stderr, "Recieved packet exceeded the buffer size\n");
		exit(1);
	} else if (ret < 0) {
		fprintf(stderr, "Failed to read from socket: %d\n", ret);
		exit(1);
	}
	return ret;
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
		fprintf(stderr, "Failed to init pubsub library: %s\n", strerror(errno));
		exit(1);
	}
	
	// create UDP socket
	memset(&udp_local_addr, 0, sizeof(udp_local_addr));
	udp_local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	udp_local_addr.sin_family = AF_INET;
	udp_local_addr.sin_port = htons(atoi(argv[2]));

	memset(&udp_addr, 0, sizeof(udp_addr));
	//udp_addr.sin_addr.s_addr = inet_addr(argv[1]);
	inet_pton(AF_INET, argv[1], &udp_addr.sin_addr);
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(atoi(argv[2]));
	udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_fd < 0) {
		fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
		exit(1);
	}
	if (bind(udp_fd, (struct sockaddr *) &udp_local_addr, sizeof(udp_local_addr)) < 0) {
		fprintf(stderr, "Failed to bind socket: %s\n", strerror(errno));
		exit(1);
	}
	
	// subscribe to everything
	spacepi_subscribe("#", sq_at_most_once, subscription_cb, NULL);
	
	pthread_create(&recv_thread, NULL, recv_callback, NULL);
	pthread_join(recv_thread, NULL);
	return 0;
	
}