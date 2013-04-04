#include "../lib/main.h"

int main() {

	bool toLog = false;

	char in_request[3000];

	setting = parse_config_file();

	if( setting->log_lvl > -1 ) {
		LogFile = openLogFile(setting->log_path);
		toLog = true;
	}

	create_and_bind();

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	for(;;) {

		if(listen(skt_lst, BACKLOG) == -1) {
			perror("in listen");
			return(EXIT_FAILURE);
		}

		if((skt_accpt = accept(skt_lst, (struct sockaddr *)&skaddr, &socksize)) == -1){
			perror("in accepting request");
			return(EXIT_FAILURE);
		}
		if(setsockopt(skt_accpt, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        	{
			printf("Cannot Set SO_RCVTIMEO for socket\n");
        	}

		if(read(skt_accpt, in_request, 3000) <= 0) {
			error_408(skt_accpt);
		}

		else {
			struct browser_request *request;
			request = parse_browser_request(in_request);

			char *rootFile = concatenation(request, setting);

			if(setting->log_lvl > 0) {
				writeConnectionLog(LogFile, request);
			}

			respond(skt_accpt, request, rootFile, toLog, LogFile);
			free(request);
		}
		close(skt_accpt);

	}

	if(close(skt_lst) == -1) {
		perror("in closing connection");
	}
	return EXIT_SUCCESS;

}

char *concatenation (struct browser_request *request, struct server_setting *setting) {

	if(strcmp(request->file_requested, "/") == 0) {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(setting->home_page)+1);
		if(rootFile == NULL) {
			perror("in malloc");	
		}
		rootFile = strcat(rootFile, setting->root_folder);
		rootFile = strcat(rootFile, setting->home_page);
		return rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("in malloc");	
		}
		rootFile = strcat(rootFile, setting->root_folder);
		rootFile = strcat(rootFile, request->file_requested);
		return rootFile;
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
