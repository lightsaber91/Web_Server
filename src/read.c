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

