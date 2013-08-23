#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/file.h>

FILE *openLogFile(char *path);

void writeConnectionLog(FILE *log, struct browser_request *request); 

void writeErrorLog(char *error, struct browser_request *request, FILE *log);
