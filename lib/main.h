#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "../src/config.c"
#include "../src/parse_request.c"
#include "../src/logger.c"
#include "../src/responses.c"

#define BACKLOG 10
#define REQ_SIZE 1000

//Settings Variable
struct server_setting *setting;
char *wurfl_location;
bool use_wurfl;

//Socket Stuff
int skt_lst, skt_accpt;
struct sockaddr_in skaddr;
socklen_t socksize;
struct timeval timeout;
int enable_keep_alive = 1;

//Log File Stuff
int LogFile;
//int nthread = 0;

void create_and_bind();

void concatenation (struct browser_request *request, struct server_setting *setting);

int read_request(int sockfd, char *buf, bool req);

void ConfigKeepAliveTimeout(int sockfd, int KeepAliveTimeout);

void config_socket(int sockfd, bool KeepAlive);

#include "../src/thread.c"
