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
#include "../src/config.c"
#include "../src/parse_request.c"
#include "../src/logger.c"
#include "../src/responses.c"

#define BACKLOG 10

//Struct for saving setting
struct server_setting *setting;

//Socket Stuff
int skt_lst, skt_accpt;
struct sockaddr_in skaddr;
socklen_t socksize;

//Log File Stuff
int LogFile;

void create_and_bind();

char *concatenation (struct browser_request *request, struct server_setting *setting);
