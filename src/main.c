#include "../lib/main.h"
#include "thread.c"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
int main() {
	bool toLog = false;

	setting = parse_config_file();

	if( setting->log_lvl > -1 ) {
		LogFile = openLogFile(setting->log_path);
		toLog = true;
	}

	create_and_bind();

	for(;;) {


		if(listen(skt_lst, BACKLOG) == -1) {
			perror("in listen");
			return(EXIT_FAILURE);
		}

		if((skt_accpt = accept(skt_lst, (struct sockaddr *)&skaddr, &socksize)) == -1){
			perror("in accepting request");
			return(EXIT_FAILURE);
		}
		struct thread_job *job = malloc(sizeof(struct thread_job));
		job->s = setting;
		job->socket = skt_accpt;
		job->toLog = toLog;
		job->LogFile = LogFile;

		pthread_create(&job->tid, NULL, my_thread, job);
/*
		timeout.tv_sec = setting->timeout;
	        timeout.tv_usec = 0;
		char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));
		config_socket(skt_accpt, setting->KeepAlive);

		while(read_request(skt_accpt, in_request) == 1) {

			struct browser_request *request;
			request = parse_browser_request(in_request);

			concatenation(request, setting);

			if(setting->log_lvl > 0) {
				writeConnectionLog(LogFile, request);
			}
			respond(skt_accpt, request, toLog, LogFile);

			free(request);
			in_request = (char *)realloc(in_request, REQ_SIZE*sizeof(char));
		}
		free(in_request);
		close(skt_accpt);
*/	}
	if(close(skt_lst) == -1) {
		perror("in closing connection");
	}
	return EXIT_SUCCESS;

}

void ConfigKeepAliveTimeout(int sockfd, int KeepAliveTimeout) {

	timeout.tv_sec = KeepAliveTimeout;
        timeout.tv_usec = 0;

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("SO_RCVTIMEO");
        }
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("SO_SNDTIMEO");
        }
}

void config_socket(int sockfd, bool KeepAlive) {

	if(KeepAlive == true) {
		if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable_keep_alive, sizeof(int)) < 0) {
			perror("SO_KEEPALIVE");
		}
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("SO_RCVTIMEO");
        }
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("SO_SNDTIMEO");
        }

}

int read_request(int sockfd, char *buf, bool req) {

	int rec = 0, byteread = 0;
	int r_max = REQ_SIZE;
	do {
		rec = recv(sockfd, buf+byteread, r_max-byteread, 0);
		if(strcmp(buf, "\r\n") != 0) {
			byteread += rec;
			if(byteread > 4) {
				if(buf[byteread-1] == '\n' && buf[byteread-2] == '\r' && buf[byteread-3] == '\n' && buf[byteread-4] == '\r') {
					return 1;
				}
			}
		}
		if(byteread == r_max) {
			r_max *= 2; 
			buf = (char *)realloc(buf, r_max*sizeof(char));
			if(buf == NULL) {
				perror("realloc");
			}
		}
		usleep(1);

	}while(rec > 0);
	if(req == true) {
		error_408(sockfd);
	}
	return 0;
}

void concatenation (struct browser_request *request, struct server_setting *setting) {

	if(strcmp(request->file_requested, "/") == 0) {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(setting->home_page)+1);
		if(rootFile == NULL) {
			perror("in malloc");
		}
		sprintf(rootFile, "%s%s", setting->root_folder, setting->home_page);
		request->file_requested = rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("in malloc");
		}
		sprintf(rootFile, "%s%s", setting->root_folder, request->file_requested);
		request->file_requested = rootFile;
	}

}

void create_and_bind() {
	int reuse = 1;
	skt_lst = socket(AF_INET,SOCK_STREAM,0);
	if(skt_lst == -1) {
                perror("in socket initialization");
                exit(EXIT_FAILURE);
        }
	socksize = sizeof(skaddr);
	memset((void *) &skaddr, 0, socksize);
	skaddr.sin_family = AF_INET;
	skaddr.sin_port = htons(setting->port);
	inet_aton((const char *)setting->ip, (struct in_addr *)&(skaddr.sin_addr.s_addr));

	if(setsockopt(skt_lst, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		perror("in setsockopt");
		exit(EXIT_FAILURE);
	}

	if(bind(skt_lst, (struct sockaddr *)&skaddr, socksize) == -1){
		perror("ip address and port probably in use!!!");
		exit(EXIT_FAILURE);
	}
}
