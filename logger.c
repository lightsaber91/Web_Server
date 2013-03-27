#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int openLogFile(char *path) {
  int log = open(path, O_CREAT | O_APPEND | O_RDWR, 0644);
	if(log == -1) {
		perror("opening log file");
	}
	return log;
}

void writeConnectionLog(int log, struct browser_request *request){

	time_t tm, local;
	struct tm *lt = malloc(sizeof(struct tm));

	time(&local);
	lt = localtime(&local);

	char *no_date = "[--/--/---- ; --:--:--] ";
	char *date = malloc(sizeof(no_date));
	char *info = malloc(sizeof(request));

	tm = mktime(lt);

	if(tm == -1) {
		perror("in mktime");
		if(write(log, no_date, strlen(no_date)) == -1) {
			perror("writing log file");
		}
		if(fsync(log) == -1) {
			perror("in fsync");
		}
	}
	else {
		sprintf(date,"[%d/%d/%d ; %d-%d-%d] ", lt->tm_mday, (lt->tm_mon)+1, (lt->tm_year)+1900, lt->tm_hour, lt->tm_min, lt->tm_sec);
		if(write(log, date, strlen(date)) == -1) {
			perror("writing log file");
		}
		if(fsync(log) == -1) {
			perror("in fsync");
		}
	}

	sprintf(info, "%s %s %s %s %s;\n", request->host, request->user_agent, request->method, request->file_requested, request->http_version);
	if(write(log, info, strlen(info)) == -1) {
		perror("writing log file");
	}
	if(fsync(log) == -1) {
		perror("in fsync");
	}
}

void writeErrorLog() {
}
