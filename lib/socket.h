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
 * Create socket and permform bind
 */
int create_and_bind(struct sockaddr_in skaddr, socklen_t socksize) {
	int reuse = 1;
	int skt_lst = socket(AF_INET,SOCK_STREAM,0);
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
	return skt_lst;
}
