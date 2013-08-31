/**
 * Merge page request from browser and browser's root folder
 * to obtain correct path of phisical file in the server
 */
char *concatenation (struct browser_request *request, struct server_setting *setting) {

	if(strcmp(request->file_requested, "/") == 0) {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(setting->home_page)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, setting->home_page) < 0) {
			perror("In sprintf: nothing written\n");
		}
		return rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, request->file_requested) < 0) {
			perror("In sprintf: nothing written\n");
		}
		return rootFile;
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

