#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct browser_request{
	char *method;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
	char *host;
	char *accept_type;
} browser_request;

void parse_request(char *line, struct browser_request *request);

void parse_first_line(char *line, struct browser_request *request);

struct browser_request *parse_browser_request(char *message);
