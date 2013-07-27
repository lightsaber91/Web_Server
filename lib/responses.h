#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "../src/parser.c"
#include "../src/resize.c"
#include "../src/wurfl_caching.c"
#include "../src/quality_caching.c"

extern char *wurfl_location;
extern bool use_wurfl;

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
void error_403(int sockfd, struct browser_request *request);
void error_404(int sockfd, struct browser_request *request);
void error_408(int sockfd);
void error_415(int sockfd);
void error_505(int sockfd);

//Send header or file
void send_header(int sockfd, char *file);
void send_file(int sockfd, char *file, char *ext);
void send_image(int sockfd, char *file, char *ext, char *user_agent, int quality);

//Generic answer
int respond(int sockfd, struct browser_request *request, bool toLog, FILE *logFile);
