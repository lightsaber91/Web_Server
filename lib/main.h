#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <signal.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "shutdown.h"
#include "../src/config.c"
#include "../src/parse_request.c"
#include "../src/logger.c"
#include "../src/mime_type.c"
#include "../src/responses.c"

#define REQ_SIZE 1024

//Socket Stuff
int skt_lst, skt_accpt;
struct sockaddr_in skaddr;
socklen_t socksize;
struct timeval timeout;

//Configuration stuff
struct server_setting *setting;
int max_conn = 250;
int conn = 0;

//Log File Stuff
FILE *LogFile;

void create_and_bind();

void concatenation (struct browser_request *request, struct server_setting *setting);

int read_request(int sockfd, char *buf, bool req);

void ConfigKeepAliveTimeout(int sockfd, int KeepAliveTimeout);

void config_socket(int sockfd, bool KeepAlive);

#include "../src/thread.c"
