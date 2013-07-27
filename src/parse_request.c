#include "../lib/parse_request.h"

int get_quality_factor(char *accept_type) {

	char *search = strstr(accept_type, "image");
	if(search != NULL) {
		double quality;
		strtok(search, "=");
		quality = atof(strtok(NULL,","))*100;
		return (int)quality;
	}
	else {
		search = strstr(accept_type, "*/*");
		double quality;
		strtok(search, "=");
		quality = atof(strtok(NULL,"\n"))*100;
		return (int)quality;
	}	
} 

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
                request->accept_quality = get_quality_factor(strtok(NULL, "\n\r"));
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

	return request;

}
