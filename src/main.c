#include "../lib/main.h"

int main() {

	signal(SIGTERM, sigterm_handler);
	
	bool toLog = false;

	setting = parse_config_file();

	if( setting->log_lvl > -1 ) {
		LogFile = openLogFile(setting->log_path);
		if(LogFile != NULL) 
			toLog = true;
		
	}
	create_and_bind();

	for(;;) {


		if(listen(skt_lst, PEND_CONNECTION) == -1) {
			perror("Socket Listening Error\n");
			return(EXIT_FAILURE);
		}

		if((skt_accpt = accept(skt_lst, (struct sockaddr *)&skaddr, &socksize)) == -1){
			perror("Socket Accepting Error\n");
			return(EXIT_FAILURE);
		}
		create_thread(setting, skt_accpt, toLog, LogFile);
	}
	if(shutdown(skt_lst, SHUT_RDWR) == -1) {
		perror("Socket Closing Error\n");
	}
	return EXIT_SUCCESS;

}

void ConfigKeepAliveTimeout(int sockfd, int KeepAliveTimeout) {

	timeout.tv_sec = KeepAliveTimeout;
        timeout.tv_usec = 0;

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }
}

void config_socket(int sockfd, bool KeepAlive) {

	if(KeepAlive == true) {
		if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable_keep_alive, sizeof(int)) < 0) {
			perror("In SetSockOpt\n");
		}
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }

}

int read_request(int sockfd, char *buf, bool req) {

	int rec = 0, byteread = 0;
	do {
		rec = recv(sockfd, buf+byteread, REQ_SIZE-byteread, MSG_NOSIGNAL);
		if(strcmp(buf, "\r\n") != 0) {
			byteread += rec;
			if(byteread > 4) {
				if(buf[byteread-3] == '\n' && buf[byteread-4] == '\r') {
					return 1;
				}
			}
		}
		usleep(1);

	}while(rec > 0);
	if(rec < 0) {
		return 0;
	}
	if(req == true) {
		error_408(sockfd);
	}
	return 0;
}

void concatenation (struct browser_request *request, struct server_setting *setting) {

	if(strcmp(request->file_requested, "/") == 0) {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(setting->home_page)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, setting->home_page) < 0) {
			perror("In sprintf: nothing written\n");
		}
		request->file_requested = rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, request->file_requested) < 0) {
			perror("In sprintf: nothing written\n");
		}
		request->file_requested = rootFile;
	}

}

void create_and_bind() {
	int reuse = 1;
	skt_lst = socket(AF_INET,SOCK_STREAM,0);
	if(skt_lst == -1) {
                perror("In socket initialization\n");
                exit(EXIT_FAILURE);
        }
	socksize = sizeof(skaddr);
	memset((void *) &skaddr, 0, socksize);
	skaddr.sin_family = AF_INET;
	skaddr.sin_port = htons(setting->port);
	inet_aton((const char *)setting->ip, (struct in_addr *)&(skaddr.sin_addr.s_addr));

	if(setsockopt(skt_lst, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		perror("In SetSockOpt\n");
		exit(EXIT_FAILURE);
	}

	if(bind(skt_lst, (struct sockaddr *)&skaddr, socksize) == -1){
		perror("IP or Port Addresses probably in Use\n");
		exit(EXIT_FAILURE);
	}
}
