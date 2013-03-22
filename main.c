#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include "./read_config.c"
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "./parse_request.c"
#include <errno.h>

static struct setting_info *setting_file;

int main() {
	setting_file = parse_config_file();
/*
	int fd = open("/var/www/prova.png", O_RDONLY);
	long lenfile = (long)lseek(fd, (off_t)0, SEEK_END);
	(void)lseek(fd, (off_t)0, SEEK_SET);
	static char header[8096];
	int ret = 0;
*/
	char request[3000];
	struct sockaddr_in skaddr;
	int skt, new_fd, reuse = 1;

        skt = socket(AF_INET,SOCK_STREAM,0);
        if(skt == -1) {
                perror("in socket initialization");
                return (EXIT_FAILURE);
        }
	socklen_t len = sizeof(skaddr);
	memset((void *) &skaddr, 0, len);
	skaddr.sin_family = AF_INET;
	skaddr.sin_port = htons(setting_file->port);
	inet_aton((const char *)setting_file->ip, (struct in_addr *)&(skaddr.sin_addr.s_addr));

	if(setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		perror("in setsockopt");
		return(EXIT_FAILURE);
	}

	if(bind(skt, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1){
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
		static struct browser_request *_request;
//		printf("%s",request);
		_request = parse_browser_request(request);
//		printf("%s\n", _request->user_agent);
/*
		(void)sprintf(header,"HTTP/1.1 200 OK\nServer: web_prova\nContent-Length: %ld\nConnection: close\nContent-Type: image/png\n\n",lenfile);
		write(new_fd, header, strlen(header));
		while (	(ret = read(fd, header, 8096)) > 0 ) {
			write(new_fd,header,ret);
		}
		sleep(1);
		//(void)lseek(fd, (off_t)0, SEEK_SET);

*/
		close(new_fd);
		free(_request);
	}

	if(close(skt) == -1) {
		perror("in closing connection");
	}
	return EXIT_SUCCESS;
}
