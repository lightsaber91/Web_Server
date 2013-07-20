#include "../lib/logger.h"

int openLogFile(char *path) {
	int log = open(path, O_CREAT | O_APPEND | O_RDWR, 0644);
	if(log == -1) {
		perror("opening log file");
	}
	return log;
}

void writeConnectionLog(int log, struct browser_request *request) {

	bool lock = false;
	do {
		if(flock(log,LOCK_EX|LOCK_NB) == -1) {
			if(errno != EWOULDBLOCK && errno != 0) {
				perror("in lock");
				return;
			}
			usleep(1);
		}
		else {
			lock = true;
		}
	}while(lock == false);

	time_t now = time(NULL);
	struct tm *lTime = localtime(&now);

	char *no_date = "[--/--/---- ; --:--:--] ";
	char date[strlen(no_date)], info[1000];

	if(now == -1) {
		perror("in mktime");
		if(write(log, no_date, strlen(no_date)) == -1) {
			perror("writing log file");
		}
	}
	else {
		if(sprintf(date,"[%d/%d/%d ; %d:%d:%d] ", lTime->tm_mday, lTime->tm_mon +1, lTime->tm_year +1900, lTime->tm_hour, lTime->tm_min, lTime->tm_sec) < 0) {
			perror("In sprintf: nothing written\n");
		}
		if(write(log, date, strlen(date)) == -1) {
			perror("writing log file");
		}
	}

	if(sprintf(info, "%s %s %s %s %s;\n", request->host, request->user_agent, request->method, request->file_requested, request->http_version) < 0) {
		perror("In sprintf: nothing written\n");
	}

	if(write(log, info, strlen(info)) == -1) {
		perror("writing log file");
	}
	bzero(info, 1000);
	if(flock(log,LOCK_UN|LOCK_NB) == -1){
		perror("in unlock");		
		return; 
	}
}

void writeErrorLog(char *error, struct browser_request *request, int log) {

	bool lock = false;
	do {
		if(flock(log,LOCK_EX|LOCK_NB) == -1) {
			if(errno != EWOULDBLOCK && errno != 0) {
				perror("in lock");
				return;
			}
			usleep(1);
		}
		else {
			lock = true;
		}
	}while(lock == false);

	char errLog[1000];
	if(strncmp(error, "404", 3) == 0) {
		if(sprintf(errLog,"Error: %s:%s;\n", error, request->file_requested) < 0) {
			perror("In sprintf: nothing written\n");
		}
	}
	else {
		if(sprintf(errLog,"Error: %s;\n", error) < 0) {
			perror("In sprintf: nothing written\n");
		}
	}
	if(write(log, errLog, strlen(errLog)) == -1) {
		perror("writing log file");
	}
	if(flock(log,LOCK_UN|LOCK_NB) == -1){
		perror("in unlock");		
		return; 
	}
}
