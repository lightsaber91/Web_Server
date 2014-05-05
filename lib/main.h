/**
 * Web Server
 * Copyright (C) 2013 - Simone Martucci <martucci.simone.91@gmail.com>
 *
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
**/

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

//Thread Stuff
pthread_mutex_t clisd_mutex = PTHREAD_MUTEX_INITIALIZER;
static int max_thread;
int n_thread = 0;

//Log File Stuff
FILE *LogFile;
int toLog = -1;
#define START "Server started successfully!"
#define EXIT "Shutting Down the Server..."

#include "../src/config.c"
//Server Setting
SETTING *setting;

#include "../src/parse_request.c"
#include "../src/logger.c"
#include "shutdown.h"
#include "util.h"
#include "socket.h"
#include "../src/mime_type.c"
#include "../src/responses.c"
#include "../src/read.c"
#include "../src/thread.c"
