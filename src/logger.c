#include "../lib/logger.h"

int openLogFile(char *path) {
	int log = open(path, O_CREAT | O_APPEND | O_RDWR, 0644);
	if(log == -1) {
		perror("opening log file");
	}
	return log;
}

void writeConnectionLog(int log, struct browser_request *request) {

	time_t now = time(NULL);
	struct tm *lTime = localtime(&now);

	char *no_date = "[--/--/---- ; --:--:--] ";
	char date[20], info[1000];

	if(now == -1) {
		perror("in mktime");
		if(write(log, no_date, strlen(no_date)) == -1) {
			perror("writing log file");
		}
		if(fsync(log) == -1) {
			perror("in fsync");
		}
	}
	else {
		sprintf(date,"[%d/%d/%d ; %d:%d:%d] ", lTime->tm_mday, lTime->tm_mon +1, lTime->tm_year +1900, lTime->tm_hour, lTime->tm_min, lTime->tm_sec);
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

void writeErrorLog(char *error, struct browser_request *request, int log) {

	char errLog[1000];
	if(strncmp(error, "404", 3) == 0) {
		sprintf(errLog,"Error: %s:%s;\n", error, request->file_requested);
	}
	else {
		sprintf(errLog,"Error: %s;\n", error);
	}
	if(write(log, errLog, strlen(errLog)) == -1) {
		perror("writing log file");
	}
	if(fsync(log) == -1) {
		perror("in fsync");
	}
}
