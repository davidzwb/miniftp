#ifndef _PARSECONF_H_
#define _PARSECONF_H_
extern char *server_addr;
extern unsigned int server_port;
extern unsigned int server_max_connfd;

extern void loadconf(char *path);

#endif
