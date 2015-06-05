#ifndef _PI_H_
#define _PI_H_

#define COMMAND_LENGTH 1024

struct sessinfo_t
{
	int ctlfd;
	int pairfd;
	char command[COMMAND_LENGTH];
	char key[COMMAND_LENGTH];
	char value[COMMAND_LENGTH];
	char user_name[64];
};

void PI(struct sessinfo_t *sessinfo);

#endif
