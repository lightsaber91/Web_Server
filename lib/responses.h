#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

//Struct used for type file
typedef struct extn{
	char *ext;
	char *mediatype;
} extn;

//Possible media types
extn extensions[] ={
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
int supported_type(char *extension, char *file);


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

//Generic answer
void respond(int sockfd, struct browser_request *request, bool toLog, int logFile);
