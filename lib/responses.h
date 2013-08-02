#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "../src/parser.c"
#include "../src/resize.c"
#include "../src/wurfl_caching.c"
#include "../src/quality_caching.c"

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

#define BUF_SIZE_0 1024
#define BUF_SIZE_1 4096
#define BUF_SIZE_2 16384
#define BUF_SIZE_3 32768



//Struct used for type file
struct extn{
	char *ext;
	char *mediatype;
};

//Possible media types
struct extn extensions[] ={
 {"gif", "image/gif" },
 {"txt", "text/plain" },
 {"jpg", "image/jpg" },
 {"jpeg","image/jpeg"},
 {"png", "image/png" },
 {"ico", "image/ico" },
 {"zip", "image/zip" },
 {"gz",  "image/gz"  },
 {"tar", "image/tar" },
 {"htm", "text/html" },
 {"html","text/html" },
 {"pdf","application/pdf"},
 {"zip","application/octet-stream"},
 {"rar","application/octet-stream"},
 {0,0} };

//Verify if file is supported from server
char *supported_type(char *file);


//Error Type:
void error_400(int sockfd);
void error_404(int sockfd);
void error_408(int sockfd);
void error_415(int sockfd);
void error_505(int sockfd);

//Send header or file
void send_header(int sockfd, char *file);
void send_file(int sockfd, char *file, char *ext);
void send_image(struct server_setting *s, int sockfd, char *file, char *ext, char *user_agent, int quality);

//Generic answer
int respond(struct server_setting *s, int sockfd, struct browser_request *request, bool toLog, FILE *logFile);
