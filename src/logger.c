#include "../lib/logger.h"

/**
 * Open Log File.
 */
FILE *openLogFile(char *path) {
	FILE *log = fopen(path, "a+");
	if(log == NULL) {
		perror("Opening Log File\n");
	}
	return log;
}

/**
 * Write on Log File information concerning connection and client request.
 */
void writeConnectionLog(FILE *log, struct browser_request *request) {

	//Obtain lock on log file
	//Flock file is thread-safe
	flockfile(log);
	//Initialize time struct
	time_t now = time(NULL);
	struct tm *lTime = localtime(&now);
	//String where info will be saved
	char *no_date = "[--/--/---- ; --:--:--] ";
	char date[strlen(no_date)], info[1000];

	if(now == -1) {
		perror("in mktime");
		if(fwrite(no_date, strlen(no_date), 1, log) != 1) {
			perror("Writing on log file\n");
		}
		fflush(log);
	}
	else {
		if(sprintf(date,"[%d/%d/%d ; %d:%d:%d] ", lTime->tm_mday, lTime->tm_mon +1, lTime->tm_year +1900, lTime->tm_hour, lTime->tm_min, lTime->tm_sec) < 0) {
			perror("In sprintf: nothing written\n");
		}
		if(fwrite(date, strlen(date), 1, log) != 1) {
			perror("Writing on log file\n");
		}
		fflush(log);
	}

	if(sprintf(info, "%s %s %s %s %s;\n", request->host, request->user_agent, request->method, request->file_requested, request->http_version) < 0) {
		perror("In sprintf: nothing written\n");
	}

	if(fwrite(info, strlen(info), 1, log) != 1) {
		perror("Writing on log file\n");
	}
	fflush(log);
	bzero(info, 1000);
	funlockfile(log);
}

/**
 * Write server error on Log.
 */
void writeErrorLog(char *error, struct browser_request *request, FILE *log) {

	flockfile(log);
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
	if(fwrite(errLog, strlen(errLog), 1, log) != 1) {
		perror("Writing on log file\n");
	}
	fflush(log);
	funlockfile(log);
}
