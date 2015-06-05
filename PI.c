//#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <crypt.h>
#include <shadow.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "util.h"

void PI(int ctlfd)//protocol interpreter
{
	int nread;
	char code[16];
	char text[1024];
	char command[1024] = "220 (miniftp0.01)\r\n"; //welcoming prompt.
	if (writen(ctlfd, command, strlen(command)) == -1)
		errquit("PI write");
	
	memset(command, 0, sizeof(command));
	nread = read(ctlfd, command, sizeof(command));//don't understand readn.
	//if (nread == 0)

	str_trim_crlf(command);
	str_char_split(command, code, text, ' ');

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
	}
}
