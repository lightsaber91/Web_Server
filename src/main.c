#include "../lib/main.h"

/**
 * Web Server main function.
 * Set signal-handler for terminating, parse configuration file and mime types file.
 * Create connections and launch threads to manage these. 
 */
int main() {
	//Initiate signal handler for sigterm
	signal(SIGTERM, sigterm_handler);
	//Read conf file and save parameters
	setting = parse_config_file();
	//Load mime types from file
	extensions = load_mime_type(setting->mime_type_file);

	bool toLog = false;
	if( setting->log_lvl > -1 ) {
		//Open Log File
		LogFile = openLogFile(setting->log_path);
		if(LogFile != NULL) 
			toLog = true;
		
	}
	if(setting->use_wurfl == true) {
		//Open wurfl file and build the xml three
		setting->doc = initDoc(setting->wurfl_location);
		if(setting->doc == NULL) {
			setting->use_wurfl = false;
		}
		setting->start = getDevicesNode(setting->doc);
		if(setting->start == NULL) {
			xmlFree(setting->doc);
			setting->use_wurfl = false;
		}
	}
	//Create socket
	create_and_bind();

	for(;;) {
		if(conn < max_conn) {
			//Take connection and launch thread to manage it
			conn++;
			if(listen(skt_lst, setting->pend_connection) == -1) {
				perror("Socket Listening Error\n");
				return(EXIT_FAILURE);
			}
	
			if((skt_accpt = accept(skt_lst, (struct sockaddr *)&skaddr, &socksize)) == -1){
				perror("Socket Accepting Error\n");
				return(EXIT_FAILURE);
			}
			create_thread(setting, skt_accpt, toLog, LogFile);
		}
		else {
			sleep(1);
		}
	}
	//Close socket and exit
	if(close(skt_lst) == -1) {
		perror("Socket Closing Error\n");
	}
	return EXIT_SUCCESS;

}

/**
 * Set the connection timeout
 */
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

/**
 * Set socketoption like keepalive, sendbuffer dimension, and keep alive timeout
 */
void config_socket(int sockfd, bool KeepAlive) {
	int reuse = 1;
	int sendbuff = 65536;
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff)) < 0) {
		perror("In SetSockOpt\n");
	}
	if(setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &reuse, sizeof(reuse)) < 0) {
		perror("In SetSockOpt\n");
	}

	if(KeepAlive == true) {
		if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &reuse, sizeof(reuse)) < 0) {
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

/**
 * Read request from the socket and save in buffer
 */
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
	if(req == true) {
		error_408(sockfd);
	}
	return 0;	 
}

/**
 * Merge page request from browser and browser's root folder
 * to obtain correct path of phisical file in the server
 */
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

/**
 * Create socket and permform bind
 */
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
		if(setting->port < 1024) {
			fprintf(stderr, "Please Use port above 1024 or launch server like root\n");
		}
		perror("IP or Port Addresses probably in Use\n");
		exit(EXIT_FAILURE);
	}
}
