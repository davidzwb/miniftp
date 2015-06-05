#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include "PI.h"

/*
struct sessinfo_t
{
	int cntlfd;
	int pairfd;
	char command[COMMAND_LENGTH];
	char key[COMMAND_LENGTH];
	char value[COMMAND_LENGTH];
};
*/
void startsession(int connfd)
{
	

	int fds[2];
	socketpair(AF_INET, SOCK_STREAM, 0, fds);//for communication between PI & nobody.

	struct sessinfo_t sessinfo;

	switch(fork())
	{
		case -1:
			perror("session fork");
		case 0://child - server
			close(fds[0]); //use fds 1;
			sessinfo.pairfd = fds[1];
			sessinfo.ctlfd = connfd;
			//PI(connfd);
			PI(&sessinfo);
		default://parent - nobody
			close(fds[1]); //use fds 0;
	}



}
