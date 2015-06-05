#include <stdio.h>
#include <string.h>
//#include "parseconf.h"

//configuration entries with default value which can be overwritten by loading from miniftp.conf.
char *server_addr = "127.0.0.1";
unsigned int server_port = 7777;
unsigned int server_max_connfd = 1; 

static struct conf_str_t {
	char *conf_name;
	char **conf_value;
}
conf_str[] = {
	{ "server_addr", &server_addr },
	{ NULL, NULL }
	};


static struct conf_uint_t {
	char *conf_name;
	unsigned int *conf_value;
}
conf_uint[] = {
	{ "server_port", &server_port },
	{ "server_max_connfd", &server_max_connfd },
	{ NULL, NULL }
	};

void loadconf(char *path)
{
	char conf_entry[64] = {};
	char key[64] = {};
	char value[64] = {};

	struct conf_str_t *conf_str_p;
	struct conf_uint_t *conf_uint_p;

	FILE *fp = fopen(path, "r");
	if (fp == NULL)
		printf("miniftp.conf file loading failed.\n");

	while (fgets(conf_entry, sizeof(conf_entry), fp) != NULL)
	{
		str_trim_lf(conf_entry);
		str_char_split(conf_entry, key, value, '=');//need more code for error avoidance.
		
		conf_str_p = conf_str;
		conf_uint_p = conf_uint;

		while (conf_str_p->conf_name)
		{
			printf("conf_str_p->conf_name: %s, key: %s, value: %s\n", conf_str_p->conf_name, key, value);
			if (strcmp(key, conf_str_p->conf_name) == 0)
			{
				if (strcmp(value, "NULL") == 0)
				{
					*conf_str_p->conf_value = NULL;
					break;
				}
				else
				{
					*conf_str_p->conf_value = strdup(value);
					break;
				}	
			}
			conf_str_p++;
		}
		
		while (conf_uint_p->conf_name)
		{
			if (strcmp(key, conf_uint_p->conf_name) == 0)
			{
				*conf_uint_p->conf_value = atoi(value);
				break;

			}
			conf_uint_p++;
		}

		memset(conf_entry, 0,sizeof(conf_entry));
		memset(key, 0,sizeof(key));
		memset(value, 0,sizeof(value));
	}
}
