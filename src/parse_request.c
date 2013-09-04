#include "../lib/parse_request.h"

/**
 * Obtain quality factor for jpg image from the browser request.
 */
int get_quality_factor(char *accept_type) {

	//The  strstr(const char *haystack, const char *needle) function 
	//finds the first occurrence of the substring needle in the 
	//string haystack.
	char *search = strstr(accept_type, "image/jpg");
	if(search != NULL) {
		double quality;
		strtok(search, "=");
		quality = atof(strtok(NULL,","))*100;
		return (int)quality;
	}
	else {
		return 100;
	}
}

/**
 * Parse single line from browser request and save the correct info in the correct variable of our struct
 */
void parse_request(char *line, HTTP_CONN *request) {

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

/**
 * For first line is used a different function, because in first line of HTTP request
 * there is three different parameters we need to save.
 */
void parse_first_line(char *line, HTTP_CONN *request) {

	request->method = strtok(line, " ");
	request->file_requested = strtok(NULL, " ");
	request->http_version = strtok(NULL, "\n\r");
}

/**
 * Allocate memory for a struct. In this struct will be saved every field needed for connections and responses.
 */
HTTP_CONN *parse_client_request(char *message) {

	HTTP_CONN *request;
	request = malloc(sizeof(HTTP_CONN));
	if(request == NULL) {
		perror("Memory Allocation Failure");
		return NULL;
	}
	char *line, *save_pointer;
	line = strtok_r(message, "\n\r", &save_pointer);
	parse_first_line(line, request);

	while((line = strtok_r(NULL, "\n\r", &save_pointer)) != NULL) {
		parse_request(line, request);
	}
	return request;
}
