#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void error_505(int sockfd) {

  char message[1000];
	char header[1000];
	if(sprintf(message, "<html><head>\n<title>505 Http Version Not Supported</title>\n</head><body>\n<h1>Http Version Not Supported</h1>\n</body></html>\n") < 0) {
		perror("in sprintf");
		return;
	} 
	if(sprintf(header, "HTTP/1.1 505 Http Version Not Supported\nServer: Prova\nContenent-Length: %ld\nConnection: close\nContent-Type: text/html\n\n",(long int) strlen(message)) < 0) {
		perror("in sprintf");
		return;
	}

	if(write(sockfd, header, strlen(header)) < 0) {
		perror("in write");
		return;
	}

	if(write(sockfd, message, strlen(message)) < 0) {
		perror("in write");
		return;
	}
	
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

	if(write(sockfd, header, strlen(header)) < 0) {
		perror("in write");
		return;
	}

	if(write(sockfd, message, strlen(message)) < 0) {
		perror("in write");
		return;
	}

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

	if(write(sockfd, header, strlen(header)) < 0) {
		perror("in write");
		return;
	}

	if(write(sockfd, message, strlen(message)) < 0) {
		perror("in write");
		return;
	}

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

	if(write(sockfd, header, strlen(header)) < 0) {
		perror("in write");
		return;
	}

	if(write(sockfd, message, strlen(message)) < 0) {
		perror("in write");
		return;
	}

}

void error_404(int sockfd, struct browser_request *request) {

	char message[1000];
	char header[1000];
	sprintf(message, "<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\n<p>The requested URL: %s was not found on this server</p>\n</body></html>\n", request->file_requested);
