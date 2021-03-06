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

//Define Errors for log file
#define LOG_400 "Error: 400 Bad Request"
#define LOG_404 "Error: 404 File Not Found"
#define LOG_408 "Error: 408 Request time-out"
#define LOG_415 "Error: 415 Unsupported Media Type"
#define LOG_505 "Error: 505 Http Version Not Supported"

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
int send_image(SETTING *s, int sockfd, char *file, char *ext, char *user_agent, int quality);

//Generic answer
int respond(SETTING *s, int sockfd, HTTP_CONN *request, char *path_to_file);
