#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util.h"
#include "debug.h"
#include "sock_util.h"
#include "timer.h"

static TIMETYPE start_time;
static TIMETYPE stop_time;

static void start_timer(void)
{
	get_time(&start_time);
}

static void stop_timer(void)
{
	get_time(&stop_time);
}

static void print_timer(void)
{
	print_duration_ms(&start_time, &stop_time);
}

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s server_address server_port num_rounds\n", argv0);
}

static void exercise(const char *name, unsigned short port, unsigned int num_rounds)
{
	unsigned int i;
	const char send_message[] = "a";
	char recv_buffer[32];
	int s;

	for (i = 0; i < num_rounds; i++) {
		s = tcp_connect_to_server(name, port);
		DIE(s < 0, "tcp_connect_to_server");

		send(s, send_message, 2, 0);
		recv(s, recv_buffer, 32, 0);

		tcp_close_connection(s);
	}
}

int main(int argc, char **argv)
{
	unsigned short port;
	unsigned int num_rounds;

	if (argc != 4) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	port = strtoul(argv[2], NULL, 10);
	num_rounds = strtoul(argv[3], NULL, 10);

	start_timer();
	exercise(argv[1], port, num_rounds);
	stop_timer();
	print_timer();

	return 0;
}
