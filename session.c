#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include "PI.h"

void startsession(int connfd)
{
	int fds[2];
	socketpair(AF_INET, SOCK_STREAM, 0, fds);//for communication between PI & nobody.

	switch(fork())
	{
		case -1:
			perror("session fork");
		case 0://child - server
			close(fds[0]); //use fds 1;
			PI(connfd);
		default://parent - nobody
			close(fds[1]); //use fds 0;
	}



}
