/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define LISTEN_PORT 12345

/* "shortcut" for struct sockaddr structure */
#define SSA			struct sockaddr

/* error printing macro */
#define ERR(call_description)                           \
	do {                                            \
		fprintf(stderr, "(%s, %d): %s\n",       \
				__FILE__, __LINE__,     \
				call_description);      \
	} while (0)

/* print error (call ERR) and exit */
#define DIE(assertion, call_description)                \
	do {                                            \
		if (assertion) {                        \
			ERR(call_description);          \
			return -1;                      \
		}                                       \
	} while(0)

/*
 * Create a server socket.
 */

static int tcp_create_listener(unsigned short port, int backlog)
{
	struct sockaddr_in address;
	int listenfd;
	int sock_opt;
	int rc;

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	DIE(listenfd < 0, "socket");

	sock_opt = 1;
	rc = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
			&sock_opt, sizeof(int));
	DIE(rc < 0, "setsockopt");

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	rc = bind(listenfd, (SSA *) &address, sizeof(address));
	DIE(rc < 0, "bind");

	rc = listen(listenfd, backlog);
	DIE(rc < 0, "listen");

	return listenfd;
}

/*
 * Use getpeername(2) to extract remote peer address. Fill buffer with
 * address format IP_address:port (e.g. 192.168.0.1:22).
 */

static int get_peer_address(int sockfd, char *buf, size_t len)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (getpeername(sockfd, (SSA *) &addr, &addrlen) < 0)
		return -1;

	sprintf(buf, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	return 0;
}

/*
 * Process input to output: output <- input + 1
 */

static void process(unsigned int input, unsigned int *output)
{
	*output = input + 1;
}

int main(int argc __attribute__((unused)),
		char *argv[] __attribute__((unused)))
{
	int srv, client;
	ssize_t n;
	unsigned int input, output;
	char addrname[128];

	srv = tcp_create_listener(LISTEN_PORT, 1);
	if (srv < 0)
		return -1;

	printf("Listening on port %d...\n", LISTEN_PORT);
	while (1) {
		client = accept(srv, NULL, 0);
		DIE(client < 0, "accept");

		get_peer_address(client, addrname, 128);
		printf("Received connection from %s\n", addrname);

		/* Receive input. */
		read(client, &input, sizeof(input));
		printf("Received: %u\n", input);

		/* Process input to output. */
		process(input, &output);

		/* Send output. */
		n = write(client, &output, sizeof(output));
		DIE(n < 0, "write");
		printf("Sent: %u\n", output);

		/* Close connection */
		close(client);
	}

	return 0;
}
