#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

char path[8] = "/var/www/";

int main(void) {
  const char ip[9] = "127.0.0.1";
	char request[3000], *line, *hostname, *useragent;
	struct sockaddr_in skaddr;
	skaddr.sin_family = AF_INET;
	skaddr.sin_port = htons(8080);
	inet_aton("127.0.0.1", &skaddr.sin_addr.s_addr);
	int skt, new_fd, reuse = 1, len = sizeof(skaddr);

	skt = socket(AF_INET,SOCK_STREAM,0);
	if(skt == -1) {
		perror("in socket initialization");
		return (EXIT_FAILURE);
	}

	if(setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		perror("in setsockopt");
		return(EXIT_FAILURE);
	}

	if(bind(skt, (struct sockaddr*)&skaddr, sizeof(skaddr)) == -1){
		perror("ip address and port probably in use!!!");
		return(EXIT_FAILURE);
	}

	for(;;) {
		if(listen(skt, 5) == -1) {
			perror("socket full");
			return(EXIT_FAILURE);
		}

		if((new_fd = accept(skt, (struct sockaddr *)&skaddr, &len)) == -1){
			perror("in accepting request");
			return(EXIT_FAILURE);
		}

		if(read(new_fd, request, 3000) == -1) {
			perror("in read");
			return EXIT_FAILURE;
		}
		line = strtok(request, "\n\r");
		hostname = strtok(NULL, "\n\r");
		useragent = strtok(NULL, "\n\r");
		printf("%s\n%s\n%s\n",line,hostname,useragent);
	}

	if(close(skt) == -1);
	return EXIT_SUCCESS;	
}
