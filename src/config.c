#include "../lib/config.h"

/**
 * Compare line read and save data in correct variable.
 */
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
	else if ( strncmp(line, "WURFL_PATH=", 11) == 0 ) {
		strtok(line, "=");
		s->wurfl_location = strtok(NULL, ";");
	}
	else if ( strncmp(line, "RESIZING=", 9 ) == 0 ) {
		if(strncmp(line, "RESIZING=ON", 11) == 0) {
			s->use_wurfl = true;
		}
		else {
			s->use_wurfl = false;
		}
	}
	else if ( strncmp(line, "PORT=", 5) == 0 ) {
		strtok(line, "=");
		s->port = atoi(strtok(NULL, ";"));
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
	else if ( strncmp(line, "KEEPALIVE=", 10) == 0 ) {

		strtok(line, "=");
		if ( strncmp(strtok(NULL, ";"), "ON", 2) == 0 ) {
			s->KeepAlive = true;
		}
		else {
			s->KeepAlive = false;
		}

	}
	else if ( strncmp(line, "TIMEOUT=", 8) == 0 ) {

		strtok(line, "=");
		s->timeout = atoi(strtok(NULL, ";"));

	}

	else if ( strncmp(line, "KEEPALIVE_REQ=", 14) == 0 ) {

		strtok(line, "=");
		s->MaxKeepAliveReq = atoi(strtok(NULL, ";"));

	}
	else if ( strncmp(line, "KEEPALIVE_TIMEOUT=", 18) == 0 ) {

		strtok(line, "=");
		s->KeepAliveTimeout = atoi(strtok(NULL, ";"));

	}
	else if ( strncmp(line, "USER_AGENT_MAX_ATTEMPTS=", 24) == 0) {

		strtok(line, "=");
		s->user_agent_max_attempts = atoi(strtok(NULL, ";"));

	}
	else if ( strncmp(line, "MIME-TYPES=", 11) == 0) {

		strtok(line, "=");
		s->mime_type_file = strtok(NULL, ";");
	}
	else if ( strncmp(line, "BACKLOG=", 8) == 0) {

		strtok(line, "=");
		s->pend_connection = atol(strtok(NULL, ";"));
	}
	else if( strncmp(line, "MAX_THREAD=", 11) == 0) {

		strtok(line, "=");
		max_thread = atoi(strtok(NULL, ";"));
	}
}
/**
 * Create a struct for server file configuration parameters.
 * Read parameters from file and save into the struct variables. 
 */
struct server_setting *parse_config_file() {

	struct server_setting *s = malloc(sizeof(struct server_setting));
	if(s == NULL) {
		perror("Memory Allocation Failure\n");
		exit(EXIT_FAILURE);
	}

	size_t n = 10;
	//Open conf file
	FILE *conf = fopen(conf_file, "r");
	if(conf == NULL) {
		perror("Configuration File NOT FOUND\n");
		exit(EXIT_FAILURE);
	}

	while(1) {

		char *line = NULL;
		//read entire line from file
		if(getdelim(&line, &n, '\n', conf) == -1) {
			perror("In getdelim\n");
		}

		if( strncmp(line, "EOF" ,3) == 0 ) {
			break;
		}

		if( strncmp(line, "#", 1) != 0 ) {
			//save parameter in struct
			write_parameter(line, s);
		}

	}
	//close file
	fclose(conf);
	return s;
}
