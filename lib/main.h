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
#include <pthread.h>

#define REQ_SIZE 1024

//Socket Stuff
struct timeval timeout;

//Configuration stuff
struct server_setting *setting;
static int max_thread;
int n_thread = 0;

//Log File Stuff
FILE *LogFile;

#include "../src/config.c"
#include "../src/parse_request.c"
#include "shutdown.h"
#include "util.h"
#include "socket.h"
#include "../src/logger.c"
#include "../src/mime_type.c"
#include "../src/responses.c"
#include "../src/read.c"
#include "../src/thread.c"
