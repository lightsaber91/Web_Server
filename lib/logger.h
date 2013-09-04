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

char *get_time_and_date();

void writeConnectionLog(HTTP_CONN *request); 

void writeInfoLog(char *error, HTTP_CONN *request);
