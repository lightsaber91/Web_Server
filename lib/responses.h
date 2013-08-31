#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "../src/parser.c"
#include "../src/resize.c"
#include "../src/wurfl_caching.c"
#include "../src/quality_caching.c"

//Defining error that should be sent
#define PAG_400 "<html><head>\n<title>400 Bad Request</title>\n</head><body>\n<h1>Bad Request</h1>\n<p>Your browser sent a request that this server could not understand</p>\n</body></html>\n"
#define ERR_400 "HTTP/1.1 400 Bad Request\nConnection: close\nContent-Type: text/html\n\n"
#define PAG_404 "<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\n<p>The requested URL was not found on this server</p>\n</body></html>\n"
#define ERR_404 "HTTP/1.1 404 Not Found\nContent-Type: text/html\nConnection: Keep-Alive\n\n"
#define PAG_408 "<html><head>\n<title>408 Request Time-out</title>\n</head><body>\n<h1>Request Time-out</h1>\n<p>Server timeout waiting for the HTTP request from the client.</p>\n</body></html>\n"
#define ERR_408 "HTTP/1.1 408 Request Time-out\nConnection: close\nContent-Type: text/html\n\n"
#define PAG_415 "<html><head>\n<title>415 Unsupported Media Type</title>\n</head><body>\n<h1>Unsupported Media Type</h1>\n<p>The requested Type is not supported by this server</p>\n</body></html>\n"
#define ERR_415 "HTTP/1.1 415 Unsupported Media Type\nContent-Type: text/html\nConnection: close\n\n"
#define PAG_505 "<html><head>\n<title>505 Http Version Not Supported</title>\n</head><body>\n<h1>Http Version Not Supported</h1>\n</body></html>\n"
#define ERR_505 "HTTP/1.1 505 Http Version Not Supported\nConnection: close\nContent-Type: text/html\n\n"

//Defining different buffer size for sending file
#define BUF_SIZE_0 256
#define BUF_SIZE_1 512
#define BUF_SIZE_2 1024
#define BUF_SIZE_3 2048
#define BUF_SIZE_4 4096
#define BUF_SIZE_5 8192
#define BUF_SIZE_6 16384

//Verify if file is supported from server
char *supported_type(char *file);

//Error Type:
void error_400(int sockfd);
void error_404(int sockfd);
void error_408(int sockfd);
void error_415(int sockfd);
void error_505(int sockfd);

//Send header, file, image
void send_header(int sockfd, char *file, char *ext);
int send_file(int sockfd, char *file, char *ext);
int send_image(struct server_setting *s, int sockfd, char *file, char *ext, char *user_agent, int quality);

//Generic answer
int respond(struct server_setting *s, int sockfd, struct browser_request *request, bool toLog, FILE *logFilezz, char *path_to_file);
