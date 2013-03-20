#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *config_path = "./config.ini";

struct setting_info {
	char *ip;
	int port;
	char *root_folder;
};

struct setting_info *parse_config_file ()
{
	struct setting_info *s;
	s = malloc(sizeof(struct setting_info));
	if(s == NULL) {
		perror("allocation memory fault");
		exit(EXIT_FAILURE);
	}

	size_t n = 50;
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
		if(strncmp(line, "#", 1) == 0)
			continue;
		else {
			if(strncmp(line, "IP=", 3) == 0) {
				int i = 3;
				char ip_t[strlen(line)-i];
				while(line[i] != ';') {
					ip_t[i-3] = line[i];
					i++;
				}
				s->ip = ip_t;
			}
      			if(strncmp(line, "PORT=", 5) == 0) {
        			int i = 5;
				char port_t[strlen(line)-i];
        			while(line[i] != ';') {
          				port_t[i-5] = line[i];
          				i++;
        			}
				s->port = atoi(port_t);
      			}
      			if(strncmp(line, "ROOT_FOLDER=", 11) == 0) {
        			int i = 11;
        			char root_t[strlen(line)-i];
        			while(line[i] != ';') {
          				root_t[i-11] = line[i];
          				i++;
        			}
        			s->root_folder = root_t;
      			}
    		}
	}
}
