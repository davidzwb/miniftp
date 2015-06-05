#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void errquit(char *str)
{
	perror(str);
	exit(0);
}

int readn(int fd, char *buf, int numread)
{
	int nread;
	int nleft = numread;
	char *ptr = buf;
	while (nleft)
	{
		nread = read(fd, ptr, nleft);
		if (nread == -1)
			errquit("read");
		else if (nread == 0)
			break;
		nleft -= nread;
		ptr += nread;
	}
	return numread - nleft;
}

int writen(int fd, char *buf, int numwrite)
{
	int nwrite;
	int nleft = numwrite;
	char *ptr = buf;
	while (nleft)
	{
		nwrite = write(fd, ptr, nleft);
		if (nwrite == -1)
			errquit("write");
		else if (nwrite == 0)
			break;
		nleft -= nwrite;
		ptr += nwrite;
	}
	return numwrite - nleft;
}


void str_trim_crlf(char *str)
{
	if (str != NULL)
	{
		int n = strlen(str);
		if (str[n - 1] == '\n' && str[n - 2] == '\r')
		{
			str[n - 1] = '\0';
			str[n - 2] = '\0';
		}
	}
}

void str_trim_lf(char *str)
{
	int n = strlen(str);
	if (str[n - 1] == '\n')
		str[n - 1] = '\0';
}

void str_char_split(char *str, char *a, char *b, char c) //split into 2 substr accoring to first occurrence of char c.
{
	char *p = str;
	if (str == NULL)
		return;
	while (*p)
	{
		if (*p == c)
			break;
		p++;
	}
	if (*p == '\0')
	{
		strcpy(a, str);
		return;
	}
	else
	{
		strncpy(a, str, p - str);
		strcpy(b, p + 1);
	}
}


