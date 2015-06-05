#ifndef _UTIL_H_
#define _UTIL_H_

void errquit(char *str);
int readn(int fd, char *buf, int numread);
int writen(int fd, char *buf, int numwrite);
void str_trim_crlf(char *str);
void str_trim_lf(char *str);
void str_char_split(char *str, char *a, char *b, char c);

#endif
