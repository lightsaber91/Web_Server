#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct browser_request{
	char *method;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
	char *host;
	int accept_quality;
};

int get_quality_factor(char *accept_type);

void parse_request(char *line, struct browser_request *request);

void parse_first_line(char *line, struct browser_request *request);

struct browser_request *parse_browser_request(char *message);
