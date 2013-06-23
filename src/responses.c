#include "../lib/responses.h"
#include "parser.c"

void error_505(int sockfd) {

	char *message = "<html><head>\n<title>505 Http Version Not Supported</title>\n</head><body>\n<h1>Http Version Not Supported</h1>\n</body></html>\n";
	char header[strlen(message)+sizeof(long int)];
	if(sprintf(header, "HTTP/1.1 505 Http Version Not Supported\nServer: Prova\nContenent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n",(long int) strlen(message)) < 0) {
		perror("in sprintf");
		return;
	}

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}

	if(send(sockfd, message, strlen(message), 0) < 0) {
		perror("in send");
		return;
	}
	close(sockfd);
}

void error_403(int sockfd, struct browser_request *request) {

	char message[1000];
	char header[1000];
	if(sprintf(message, "<html><head>\n<title>403 Forbidden</title>\n</head><body>\n<h1>Forbidden</h1>\n<p>You don't have permission to access: %s</p>\n</body></html>\n", request->file_requested) < 0) {
		perror("in sprintf");
		return;
	} 
	if(sprintf(header, "HTTP/1.1 403 Forbidden\nServer: Prova\nContenent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n",(long int) strlen(message)) < 0) {
		perror("in sprintf");
		return;
	}

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}

	if(send(sockfd, message, strlen(message), 0) < 0) {
		perror("in send");
		return;
	}
	close(sockfd);
}

void error_400(int sockfd) {

	char message[1000];
	char header[1000];
	if(sprintf(message, "<html><head>\n<title>400 Bad Request</title>\n</head><body>\n<h1>Bad Request</h1>\n<p>Your browser sent a request that this server could not understand</p>\n</body></html>\n") < 0) {
		perror("in sprintf");
		return;
	} 
	if(sprintf(header, "HTTP/1.1 400 Bad Request\nServer: Prova\nContenent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n", (long int) strlen(message)) < 0) {
		perror("in sprintf");
		return;
	}

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}

	if(send(sockfd, message, strlen(message), 0) < 0) {
		perror("in send");
		return;
	}
	close(sockfd);
}

void error_408(int sockfd) {

	char message[1000];
	char header[1000];
	if(sprintf(message, "<html><head>\n<title>408 Request Time-out</title>\n</head><body>\n<h1>Request Time-out</h1>\n<p>Server timeout waiting for the HTTP request from the client.</p>\n</body></html>\n") < 0) {
		perror("in sprintf");
		return;
	} 
	if(sprintf(header, "HTTP/1.1 408 Request Time-out\nServer: Prova\nContenent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n", (long int) strlen(message)) < 0) {
		perror("in sprintf");
		return;
	}

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}

	if(send(sockfd, message, strlen(message), 0) < 0) {
		perror("in send");
		return;
	}
	close(sockfd);

}

void error_404(int sockfd, struct browser_request *request) {

	char message[1000];
	char header[1000];
	char *ap = malloc(strlen(request->file_requested));
	strcpy(ap, request->file_requested);
	strtok(ap, "//");
	strtok(NULL, "//");
	ap = strtok(NULL, "\n");
	sprintf(message, "<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\n<p>The requested URL: %s was not found on this server</p>\n</body></html>\n", ap);
	sprintf(header, "HTTP/1.1 404 Not Found\nServer: Prova\nContenent-Length: %ld\nContent-Type: text/html\nConnection: Keep-Alive\n\n", (long int) strlen(message));

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}
	if(send(sockfd, message, strlen(message), 0) < 0 ) {
		perror("in send");
		return;
	}
	//close(sockfd);
}

void error_415(int sockfd) {

	char message[1000];
	char header[1000];
	sprintf(message, "<html><head>\n<title>415 Unsupported Media Type</title>\n</head><body>\n<h1>Unsupported Media Type</h1>\n<p>The requested Type is not supported by this server</p>\n</body></html>\n");
	sprintf(header, "HTTP/1.1 415 Unsupported Media Type\nServer: Prova\nContenent-Length: %ld\nContent-Type: text/html\nConnection: close\n\n", (long int) strlen(message));

	if(send(sockfd, header, strlen(header), 0) < 0) {
		perror("in send");
		return;
	}
	if(send(sockfd, message, strlen(message), 0) < 0 ) {
		perror("in send");
		return;
	}
	close(sockfd);
}

void send_header(int sockfd, char *file) {
	int fd;
	fd = open(file, O_RDONLY);
	if(fd == -1){
		perror("in open");
		exit(EXIT_FAILURE);
	}
	long lenfile;
	lenfile = (long)lseek(fd, (off_t)0, SEEK_END);
	if(lenfile == -1){
		perror("in lseek");
		exit(EXIT_FAILURE);
	}
	if(lseek(fd, (off_t)0, SEEK_SET) == -1) {
		perror("in lseek");
		exit(EXIT_FAILURE);
	}
	char header[8096];
	sprintf(header,"HTTP/1.1 200 OK\nServer: web_prova\nContent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n",lenfile);
	if(send(sockfd, header, strlen(header), 0) == -1) {
		perror("in send");
		return;
	}
	close(sockfd);
}

void send_file(int sockfd, char *file, char *ext) {
	int fd;
	fd = open(file, O_RDONLY);
	if(fd == -1){
		perror("in open");
		exit(EXIT_FAILURE);
	}
	long lenfile;
	lenfile = (long)lseek(fd, (off_t)0, SEEK_END);
	if(lenfile == -1){
		perror("in lseek");
		exit(EXIT_FAILURE);
	}
	if(lseek(fd, (off_t)0, SEEK_SET) == -1) {
		perror("in lseek");
		exit(EXIT_FAILURE);
	}
	int ret = 0;
	char header[250];
	sprintf(header,"HTTP/1.1 200 OK\nServer: web_prova\nContent-Length: %ld\nConnection: Keep-Alive\nContent-Type: %s\n\n",lenfile, ext);
	if(send(sockfd, header, strlen(header), 0) == -1) {
		perror("sending file");
		return;
	}
	while ((ret = read(fd, header,250)) > 0 ){
		if(send(sockfd,header,ret, 0) == -1) {
			perror("in send");
			return;
		}
	}
}

void send_image(int sockfd, char *file, char *ext, char *user_agent) {

	if(parse_UA(user_agent) == 0) {
		char *resized = (char *)resize(file);
		send_file(sockfd, resized, ext);
	}
	else
		send_file(sockfd, file, ext);
}

int respond(int sockfd, struct browser_request *request, bool toLog, int logFile) {


	if(strcmp(request->http_version, "HTTP/1.0") != 0 && strcmp(request->http_version, "HTTP/1.1") != 0) {
		error_505(sockfd);
		if(toLog) {
			writeErrorLog("505 Http Version Not Supported", request, logFile);
		}
		return -1;
	}

	else if(strncmp(request->file_requested, "../", 3) == 0) {
		error_403(sockfd, request);
		if(toLog) {
			writeErrorLog("403 Forbidden", request, logFile);
		}
		return -1;
	}

	else if(strcmp(request->method, "GET") != 0 && strcmp(request->method, "HEAD") != 0  && strcmp(request->method, "get") != 0  && strcmp(request->method, "head") != 0) {
		error_400(sockfd);
		if(toLog) {
			writeErrorLog("400 Bad Request", request, logFile);
		}
		return -1;
	}

	else {
		char *extension = supported_type(request->file_requested);

		if(extension == NULL) {
			error_415(sockfd);
			if(toLog) {
				writeErrorLog("415 Unsupported Media Type", request, logFile);
			}
			return -1;
		}

		else if(access(request->file_requested, R_OK) == -1) {
			error_404(sockfd, request);
			if(toLog) {
				writeErrorLog("404 File Not Found", request, logFile);
			}
			return 0;
		}

		else if(strcmp(request->method, "HEAD") == 0 || strcmp(request->method, "head") == 0) {
			send_header(sockfd, request->file_requested);
			return -1;
		}

		else {
			if(strncmp(extension, "image", 5) == 0 && use_wurfl == true) {
				send_image(sockfd, request->file_requested, extension, request->user_agent);
			}
			else 
				send_file(sockfd, request->file_requested, extension);
			return 0;
		}
	}
	return -1;
}

char *supported_type(char *file) {
	char *extension = malloc(1);
	char *ap = strrchr(file, '.');
	if(ap == NULL) {
		return "";
	}
	char *type = malloc(strlen(ap));
	sprintf(type, "%s", ap+1);
	int i;
	if(type == NULL)
		return "";
	for(i = 0; extensions[i].ext != NULL; i++) {
		if (strcmp(type, extensions[i].ext) == 0) {
			extension = realloc(extension, sizeof(extensions[i].mediatype)); 
			extension = extensions[i].mediatype;
			return extension;
		}
	}
	return NULL;
}
