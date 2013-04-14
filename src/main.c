#include "../lib/main.h"

int main() {

	bool toLog = false;

	setting = parse_config_file();

	if( setting->log_lvl > -1 ) {
		LogFile = openLogFile(setting->log_path);
		toLog = true;
	}

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;


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
		char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));
		config_socket(skt_accpt);

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
	}

	if(close(skt_lst) == -1) {
		perror("in closing connection");
	}
	return EXIT_SUCCESS;

}

void config_socket(int sockfd) {

	if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &enable_keep_alive, sizeof(int)) < 0) {
		perror("SO_KEEPALIVE");
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("SO_RCVTIMEO");
        }

}

int read_request(int sockfd, char *buf) {

	int rec = 0, byteread = 0;
	int r_max = REQ_SIZE;
	do {
		rec = recv(sockfd, buf+byteread, r_max-byteread, 0);
		byteread += rec;
		if(buf[byteread-1] == '\n' && buf[byteread-2] == '\r' && buf[byteread-3] == '\n' && buf[byteread-4] == '\r') {
			return 1;
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
	if(sockfd == fcntl(sockfd, F_GETFD)) {
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
		rootFile = strcat(rootFile, setting->root_folder);
		rootFile = strcat(rootFile, setting->home_page);
		request->file_requested = rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("in malloc");
		}
		rootFile = strcat(rootFile, setting->root_folder);
		rootFile = strcat(rootFile, request->file_requested);
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
