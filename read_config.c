#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *config_path = "./config.txt";

struct setting_info {
	char *ip;
	int port;
	char *root_folder;
	int max_process;
	int min_process;
	int max_thread;
	int min_thread;
};

struct setting_info *parse_config_file ()
{
	size_t n = 100;
	struct setting_info *s;
	s = malloc(sizeof(struct setting_info));

	if(s == NULL) {
		perror("allocation memory fault");
		exit(EXIT_FAILURE);
	}

	char *line = NULL;

	FILE *config = fopen(config_path, "r");
	if(config == NULL) {
		perror("Fail opening configuration file");
		exit(EXIT_FAILURE);
	}

	for(;;) {
		getline(&line, &n, config);
		if(strncmp(line, "EOF", 3) == 0)
			return s;
		if(strncmp(line, "#", 1) == 0);
		else {
 			if(strncmp(line, "PORT=", 5) == 0) {
				strtok(line, "=");
        			char *port_t = strtok(NULL, ";");
				s->port = atoi(port_t);
      			}
     			else if(strncmp(line, "ROOT_FOLDER=", 12) == 0) {
				strtok(line, "=");
				s->root_folder = strtok(NULL, ";");
      			}
                        else if(strncmp(line, "IP=", 3) == 0) {
				strtok(line, "=");
				s->ip = strtok(NULL, ";");
                        }

    		}
	}
}
