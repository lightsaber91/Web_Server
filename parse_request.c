#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct browser_request {
	char *method;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
	char *host;
	char *accept_type;
};

void parse_request(char *line, struct browser_request *request) {
        if(strncmp(line, "User-Agent: ", 12) == 0) {
                strtok(line, " ");
		request->user_agent = strtok(NULL, "\n\r");
        }
	else if(strncmp(line, "Connection: ", 12) == 0) {
		strtok(line, " ");
		request->connection_type = strtok(NULL, "\n\r");
	}
        else if(strncmp(line, "Host: ", 6) == 0) {
                strtok(line, " ");
                request->host = strtok(NULL, "\n\r");
        }
        else if(strncmp(line, "Accept: ", 8) == 0) {
                strtok(line, " ");
                request->accept_type = strtok(NULL, "\n\r");
        }
}

void parse_first_line(char *line, struct browser_request *request) {
	request->method = strtok(line, " ");
	request->file_requested = strtok(NULL, " ");
	request->http_version = strtok(NULL, "\n\r");
}

struct browser_request *parse_browser_request(char *message) {
	struct browser_request *request;
	request = malloc(sizeof(struct browser_request));
	if(request == NULL) {
		perror("memory allocation Failure");
		exit(EXIT_FAILURE);
	}
	char *line, *save_pointer;
	line = strtok_r(message, "\n\r", &save_pointer);
	parse_first_line(line, request);
	while((line = strtok_r(NULL, "\n\r", &save_pointer)) != NULL) {
		parse_request(line, request);
	}
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n", request->method, request->file_requested, request->user_agent, request->connection_type, request->http_version, request->host, request->accept_type);
	return request;
}
