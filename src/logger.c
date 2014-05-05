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
 * Try to obtain time and date and write info in a string
 */
char *get_time_and_date() {
	//Initialize time struct
	time_t now = time(NULL);
	struct tm *lTime = localtime(&now);
	//String where info will be saved
	char *no_date = "[--/--/----;--:--:--]";
	char *date = malloc(strlen(no_date)+1);
	if(now == -1 || date == NULL) {
		free(date);
		return no_date;
	}
	else {
		if(sprintf(date,"[%d/%d/%d;%d:%d:%d]", lTime->tm_mday, lTime->tm_mon +1, lTime->tm_year +1900, lTime->tm_hour, lTime->tm_min, lTime->tm_sec) < 0) {
			perror("In sprintf: nothing written\n");
			free(date);
			return no_date;
		}
		return date;
	}
}

/**
 * Write on Log File information concerning connection and client request.
 */
void writeConnectionLog(HTTP_CONN *request) {

	//Obtain lock on log file
	pthread_mutex_lock(&clisd_mutex);
	char *info = malloc(1000*sizeof(char));
	char *date = get_time_and_date();

	if(sprintf(info, "%s Thread %lu | Request: %s %s %s - Client User-Agent: %s --> Sending file on socket...\n", date, pthread_self(),request->method, request->file_requested, request->http_version, request->user_agent) < 0) {
		perror("In sprintf: nothing written\n");
	}

	if(fwrite(info, strlen(info), 1, LogFile) != 1) {
		perror("Writing on log file\n");
	}
	fflush(LogFile);
	free(date);
	free(info);
	//Release lock
	pthread_mutex_unlock(&clisd_mutex);
}

/**
 * Write server error on Log.
 */
void writeInfoLog(char *error, HTTP_CONN *request) {

	pthread_mutex_lock(&clisd_mutex);
	char *errLog = malloc(500*sizeof(char));
	char *date = get_time_and_date();
	if(request != NULL) { 
		if(sprintf(errLog,"%s Thread %lu | %s --> %s;\n", date, pthread_self(), error, request->file_requested) < 0) {
			perror("In sprintf: nothing written\n");
		}
	}
	else {
		if(sprintf(errLog,"%s Thread %lu | %s;\n", date, pthread_self(), error) < 0) {
			perror("In sprintf: nothing written\n");
		}
	}
	if(fwrite(errLog, strlen(errLog), 1, LogFile) != 1) {
		perror("Writing on log file\n");
	}
	fflush(LogFile);
	free(errLog);
	free(date);
	pthread_mutex_unlock(&clisd_mutex);
}
