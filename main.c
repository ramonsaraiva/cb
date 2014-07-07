#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#define MAXLINE 4096

int con(char* sv, unsigned int port, int *sfd)
{
	struct sockaddr_in sv_addr;
	bzero(&sv_addr, sizeof(sv_addr));
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_port = htons(port);

	if ((*sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return 0;

	if (inet_pton(AF_INET, sv, &sv_addr.sin_addr) <= 0)
		return 0;

	if (connect(*sfd, (struct sockaddr*)&sv_addr, sizeof(sv_addr)) < 0)
		return 0;

	return 1;
}

int ssend(int sfd, char* out, int debug)
{
	if (debug)
		printf("OUT: %s", out);
	return send(sfd, out, strlen(out), 0);
}

int sread(int sfd, char* in, int debug)
{
	int n;
	n = read(sfd, in, MAXLINE);
	if (n > 0 && debug)
		printf("IN: %s", in);
	return n;
}

int main(int argc, char** argv)
{
	int sfd;
	int n;

	char in[MAXLINE+1];
	char out[MAXLINE+1];
	char* cmd;
	char* pos;

	char* sv = argv[1];
	int debug = 1;

	if (!con(sv, 6667, &sfd))
	{
		printf("Failed to connect to %s\n", sv);
		exit(1);
	}

	ssend(sfd, "NICK CCCCC\r\n", debug);
	ssend(sfd, "USER CCCCC 8 * : CBOTCBOTCBOT\r\n", debug);

	while (1)
	{
		in[0] = 0;
		n = sread(sfd, in, debug);

		if (n <= 0)
			continue;

		in[n] = 0;
	}
}
