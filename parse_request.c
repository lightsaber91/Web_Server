#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct browser_request {
  char *metod;
	char *file_requested;
	char *user_agent;
	char *connection_type;
	char *http_version;
};

void parse_request(char *line, struct browser_request *request) {
        char *usless;
        if(strncmp(line, "User-Agent: ", 12) == 0) {
                usless = strtok(line, ": ");
                request->user_agent = strtok(NULL, "\n\r");
        }
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
	while((line = strtok_r(NULL, "\n\r", &save_pointer)) != NULL) {
		printf("%s\n", line);
		parse_request(line, request);
	}
	return request;
}
