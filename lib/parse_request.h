#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct client_request{
	char *method;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
	char *host;
	int accept_quality;
}HTTP_CONN;

int get_quality_factor(char *accept_type);

void parse_request(char *line, HTTP_CONN *request);

void parse_first_line(char *line, HTTP_CONN *request);

HTTP_CONN *parse_client_request(char *message);
