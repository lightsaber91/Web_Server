#include "../lib/config.h"

void write_parameter(char *line, struct server_setting *s) {

	if ( strncmp(line, "IP=", 3) == 0 ) {
		strtok(line, "=");
		s->ip = strtok(NULL, ";");
	}
	else if ( strncmp(line, "ROOT_FOLDER=", 12 ) == 0) {
		strtok(line, "=");
		s->root_folder = strtok(NULL, ";");
	}
	else if ( strncmp(line, "HOME_PAGE=", 10 ) == 0 ) {
		strtok(line, "=");
		s->home_page = strtok(NULL, ";");
	}
	else if ( strncmp(line, "LOG_PATH=", 9 ) == 0 ) {

		if ( strncmp(line, "LOG_PATH=;", 10 ) == 0) {
			s->log_path = "./server.log";
		}
		else {
			strtok(line, "=");
			s->log_path = strtok(NULL, ";");
		}
	}
	else if ( strncmp(line, "PORT=", 5) == 0 ) {
		strtok(line, "=");
		char *port_t = strtok(NULL, ";");
		s->port = atoi(port_t);
	}
	else if ( strncmp(line, "LOG_LEVEL=", 10) == 0 ) {

		strtok(line, "=");
		char *lvl_t = strtok(NULL, "\n");
		if( strncmp(lvl_t, ";", 1) == 0 || strncmp(lvl_t, "NONE", 4) == 0 ) {
			s->log_lvl = -1;
		}
		else if( strncmp(lvl_t, "ERROR", 5) == 0 ) {
			s->log_lvl = 0;
		}
		else if( strncmp(lvl_t, "DEBUG;", 6) == 0 ) {
			s->log_lvl = 1;
		}

	}

}

struct server_setting *parse_config_file() {

	struct server_setting *s = malloc(sizeof(struct server_setting));

	if(s == NULL) {
		perror("allocating memory");
		exit(EXIT_FAILURE);
	}

	size_t n = 10;

	FILE *conf = fopen(conf_file, "r");
	if(conf == NULL) {
		perror("config file not found");
		exit(EXIT_FAILURE);
	}

	while(1) {

		char *line = NULL;

		if(getdelim(&line, &n, '\n', conf) == -1) {
			perror("in getdelim");
			//exit(EXIT_FAILURE);
		}

		if( strncmp(line, "EOF" ,3) == 0 ) {
			break;
		}

		if( strncmp(line, "#", 1) != 0 ) {
			write_parameter(line, s);
		}

	}

	return s;
}
