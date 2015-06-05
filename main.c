#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "session.h"
#include "parseconf.h"

#define BACKLOG 1

 
int startserver(char *addr, int port)
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (addr != NULL)
	{
		if (inet_pton(AF_INET, addr, &servaddr.sin_addr) == 0)
			perror("addr error");
	}
	else
		servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		perror("bind");

	if (listen(listenfd, BACKLOG) == -1)
		perror("listen");

	return listenfd;	
}

int main(int argc, char *argv[])
{
	pid_t pid;

	int listenfd;
	int connfd;
	
	socklen_t len;
	struct sockaddr_in cliaddr;

	loadconf("miniftp.conf");
	printf("server_addr: %s\n", server_addr);
	printf("server_port: %d\n", server_port);
	printf("server_max_connfd: %d\n", server_max_connfd);


	if (geteuid() != 0)//need root privilege.
	{
		printf("please use root privilege.\n");
		exit(0);
	}

	listenfd = startserver(NULL, 7777);

	len = sizeof(cliaddr);
	if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len)) == -1)
		perror("accept");
	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid == 0)//child
	{
		startsession(connfd);
	}

	wait(NULL); //wait for child to terminate.

	return 0;
}
