//#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <shadow.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "util.h"
#include "PI.h"
/*
struct struct sessinfo_t
{
	int cntlfd;
	int pairfd;
	char command[COMMAND_LENGTH];
	char key[COMMAND_LENGTH];
	char value[COMMAND_LENGTH];
};
*/
//extern struct struct sessinfo_t sessinfo;


void ftp_reply(int fd, int code, char *comment)
{
	int nwrite;
	char buf[1024];
	sprintf(buf, "%d %s\r\n", code, comment);
	nwrite = write(fd, buf, strlen(buf));
	if (nwrite == -1)
		errquit(buf);
}


void do_user(struct sessinfo_t *sessinfo)
{
	memset(sessinfo->user_name, 0, sizeof(sessinfo->user_name));
	strcpy(sessinfo->user_name, sessinfo->value);
	ftp_reply(sessinfo->ctlfd, 331, "Please specify the password.");
}
	
void do_pass(struct sessinfo_t *sessinfo)
{
	if (sessinfo->user_name == NULL)
		errquit("user_name");
	struct passwd *pw = getpwnam(sessinfo->user_name);
	if (pw == NULL)
	{
		ftp_reply(sessinfo->ctlfd, 530, "Login incorrect.");
		exit(0);
	}
	char *encrypted;
	struct spwd *spw;
	spw = getspnam(sessinfo->user_name);
	if (spw == NULL)
		errquit("getspnam");
	encrypted = crypt(sessinfo->value, spw->sp_pwdp);
	if (strcmp(encrypted, spw->sp_pwdp) == 0)
	{
		ftp_reply(sessinfo->ctlfd, 230, "Login successful");
	}
	else
	{
		ftp_reply(sessinfo->ctlfd, 530, "testLogin incorrect.");
		exit(0);
	}
}


struct ftp_command_t
{
	char *name;
	void (*func)(struct sessinfo_t *sessinfo);
}
ftp_command[] =
{
	{"USER", do_user},
	{"PASS", do_pass}
};



void PI(struct sessinfo_t *sessinfo)//protocol interpreter
{
	int nread;
	int len;
	//char code[16];
	//char text[1024];
	//char command[1024] = "220 (miniftp0.01)\r\n"; //welcoming prompt.
	ftp_reply(sessinfo->ctlfd, 220, "(miniftp0.01)"); //welcoming prompt.
	//if (writen(sessinfo->ctlfd, sessinfo->command, strlen(sessinfo->command)) == -1)
	//	errquit("PI write");
	
	//memset(command, 0, sizeof(command));
	len = sizeof(ftp_command) / sizeof(ftp_command[0]);
	while (1)
	{
		memset(sessinfo->command, 0, sizeof(sessinfo->command));
		memset(sessinfo->key, 0, sizeof(sessinfo->key));
		memset(sessinfo->value, 0, sizeof(sessinfo->value));

		nread = read(sessinfo->ctlfd, sessinfo->command, sizeof(sessinfo->command));//don't understand readn yet.
		if (nread <= 0)
			errquit("read");

		str_trim_crlf(sessinfo->command);
		str_char_split(sessinfo->command, sessinfo->key, sessinfo->value, ' ');
		
		int i = 0;
		for (i = 0; i < len; i++)
		{
			if (strcmp(ftp_command[i].name, sessinfo->key) == 0)
			{	
				ftp_command[i].func(sessinfo);//jump to corresponding function.
				break;
			}
		}
		if (i == len)
			ftp_reply(sessinfo->ctlfd, 502, "Command not implemented.");
	}
		/*

	if (strcmp(code, "USER") == 0)//switch to specified user account;
	{
		struct passwd *pw = getpwnam(text);
		//setegid(pw->pw_gid);
		//seteuid(pw->pw_uid);
		memset(command, 0, sizeof(command));
		sprintf(command, "%s\r\n", "331 Please specify the password.");
		if (write(ctlfd, command, strlen(command)) <= 0)
			errquit("write 331");

		memset(command, 0, sizeof(command));
		if (read(ctlfd, command, sizeof(command)) <= 0)//read passwd
			errquit("read passwd");

		memset(code, 0, sizeof(code));
		memset(text, 0, sizeof(text));
		str_trim_crlf(command);
		str_char_split(command, code, text, ' ');

		if (strcmp(code, "PASS") == 0)
		{
			char *encrypted, *p;
			struct spwd * spw;
			if (pw == NULL)//wrong user name.
			{
				write(ctlfd, "530 Login incorrect.\r\n", sizeof("530 Login incorrect.\r\n"));
				exit(0);
			}
			spw = getspnam(pw->pw_name);//get shadow passwd
			encrypted = crypt(text, spw->sp_pwdp);
			p = text;
			while (*p)
				*p++ = '\0'; // erase unencrypted passwd.

			if (strcmp(spw->sp_pwdp, encrypted) == 0)//correct passwd.
			{
				printf("gid=%d, uid=%d.\n", geteuid(), getegid());
				setegid(pw->pw_gid);
				seteuid(pw->pw_uid);
				chdir(pw->pw_pw_dir);
				printf("gid=%d, uid=%d.\n", geteuid(), getegid());
				write(ctlfd, "230 Login successful.\r\n", sizeof("230 Login successfully.\r\n"));
			}
			else
			{
				write(ctlfd, "530 Login incorrect.\r\n", sizeof("530 Login incorrect.\r\n"));
				exit(0);
			}
		}
	}	*/


}
