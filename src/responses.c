#include "../lib/responses.h"

/**
 * According to file length ,choose dimension of send buffer.
 */
int choose_buf_size(unsigned long long len) {
	if(len/BUF_SIZE_0 < 5)
		return BUF_SIZE_0;
	if(len/BUF_SIZE_1 < 5)
		return BUF_SIZE_1;
	if(len/BUF_SIZE_2 < 5)
		return BUF_SIZE_2;
	if(len/BUF_SIZE_3 < 5)
		return BUF_SIZE_3;
	else
		return BUF_SIZE_4;
}

/**
 * Send error 505: Http Version not supported.
 */
void error_505(int sockfd) {

	if(send(sockfd, ERR_505, strlen(ERR_505), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}

	if(send(sockfd, PAG_505, strlen(PAG_505), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}
}

/**
 * Send Error 400: Bad Request.
 */
void error_400(int sockfd) {

	if(send(sockfd, ERR_400, strlen(ERR_400), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}

	if(send(sockfd, PAG_400, strlen(PAG_400), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}
	close(sockfd);
}

/**
 * Send Error 408: Request Time-out.
 */
void error_408(int sockfd) {

	if(send(sockfd, ERR_408, strlen(ERR_408), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}

	if(send(sockfd, PAG_408, strlen(PAG_408), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}
}

/**
 * Send Error 404: File not Found.
 */
void error_404(int sockfd) {

	if(send(sockfd, ERR_404, strlen(ERR_404), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}
	if(send(sockfd, PAG_404, strlen(PAG_404), MSG_NOSIGNAL) < 0 ) {
		perror("Sending Packet\n");
		return;
	}
}

/**
 * Send Error 415: Unsupported Media Type.
 */
void error_415(int sockfd) {

	if(send(sockfd, ERR_415, strlen(ERR_415), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet\n");
		return;
	}
	if(send(sockfd, PAG_415, strlen(PAG_415), MSG_NOSIGNAL) < 0 ) {
		perror("Sending Packet\n");
		return;
	}
}

/**
 * Send HTTP header of file.
 */
void send_header(int sockfd, char *file) {
	int fd = open(file, O_RDONLY, S_IREAD | S_IWRITE);
	if(fd == -1){
		perror("Opening File Requested\n");
		return;
	}
	unsigned long long lenfile = (unsigned long long)lseek(fd, (off_t)0, SEEK_END);
	if(lenfile == (unsigned long long)-1){
		perror("In lseek\n");
		return;
	}
	if(lseek(fd, (off_t)0, SEEK_SET) == -1) {
		perror("In lseek\n");
		return;
	}
	char header[BUF_SIZE_0];
	if(sprintf(header,"HTTP/1.1 200 OK\nContent-Length: %lld\nConnection: close\nContent-Type: text/html\n\n",lenfile) < 0) {
		perror("In sprintf: nothing written\n");
		return;
	}
	if(send(sockfd, header, strlen(header), MSG_NOSIGNAL) == -1) {
		perror("Sending Packet\n");
		return;
	}
	close(fd);
}

/**
 * General function that send every file supported by the server.
 */
void send_file(int sockfd, char *file, char *ext) {
	int fd = open(file, O_RDONLY, S_IREAD | S_IWRITE);
	if(fd == -1){
		perror("Opening File Requested\n");
		return;
	}
	unsigned long long lenfile;
	lenfile = (unsigned long long)lseek(fd, (off_t)0, SEEK_END);
	if(lenfile == (unsigned long long) -1){
		perror("In lseek\n");
		return;
	}
	if(lseek(fd, (off_t)0, SEEK_SET) == -1) {
		perror("In lseek\n");
		return;
	}
	int ret = 0;
	int size = choose_buf_size(lenfile);
	char header[size];
	if(sprintf(header,"HTTP/1.1 200 OK\nContent-Length: %lld\nConnection: Keep-Alive\nContent-Type: %s\n\n", lenfile, ext) < 0) {
		perror("In sprintf: nothing written\n");
		return;
	}
	if(send(sockfd, header, strlen(header), MSG_NOSIGNAL) == -1) {
		perror("Sending Packet\n");
		return;
	}
	while ((ret = read(fd, header, size)) > 0 ){
		if(send(sockfd, header, ret, MSG_NOSIGNAL /*| MSG_MORE*/) == -1) {
			perror("Sending Packet\n");
			close(fd);
			return;
		}
	}
	close(fd);
}

/**
 * This function resize image if enabled in setting, than call send_file function,
 * otherwise call send_file on image without resizing it.
 */
void send_image(struct server_setting *s, int sockfd, char *file, char *ext, char *user_agent, int quality) {

	if(s->use_wurfl == true) {
		struct device_property *property = malloc(sizeof(struct device_property));
		if(property != NULL) { 
			if(parse_UA(user_agent, property, s->start) == 0) {
				char *file_resized = cache_by_resolution(property->resolution_width, property->resolution_height, file);
				send_file(sockfd, file_resized, ext);
			}
			else {
				char *lowq_file = cache_by_quality(file, quality); 
				send_file(sockfd, lowq_file, ext);
			}
		}
		else {
			char *lowq_file = cache_by_quality(file, quality); 
			send_file(sockfd, lowq_file, ext);
		}
	}
	else { 
		send_file(sockfd, file, ext);
	}
}

/**
 * Analize http field in parsed request, in case of errors send the correct error message,
 * in case of HEADER request send only the header and in case of GET send the file.
 * If error occurs writes in the log file. 
 */
int respond(struct server_setting *s, int sockfd, struct browser_request *request, bool toLog, FILE *logFile) {


	if(strcasecmp(request->http_version, "HTTP/1.0") != 0 && strcasecmp(request->http_version, "HTTP/1.1") != 0) {
		error_505(sockfd);
		if(toLog) {
			writeErrorLog("505 Http Version Not Supported", request, logFile);
		}
		return -1;
	}

	else if(strcasecmp(request->method, "GET") != 0 && strcasecmp(request->method, "HEAD") != 0) {
		error_400(sockfd);
		if(toLog) {
			writeErrorLog("400 Bad Request", request, logFile);
		}
		return -1;
	}

	else {
		char *extension = supported_type(request->file_requested);

		if(extension == NULL) {
			error_415(sockfd);
			if(toLog) {
				writeErrorLog("415 Unsupported Media Type", request, logFile);
			}
			return -1;
		}

		else if(access(request->file_requested, R_OK) == -1) {
			error_404(sockfd);
			if(toLog) {
				writeErrorLog("404 File Not Found", request, logFile);
			}
			return 0;
		}

		else if(strcmp(request->method, "HEAD") == 0 || strcmp(request->method, "head") == 0) {
			send_header(sockfd, request->file_requested);
			return 0;
		}

		else {
			if(strncmp(extension, "image", 5) == 0) {
				send_image(s, sockfd, request->file_requested, extension, request->user_agent, request->accept_quality);
			}
			else 
				send_file(sockfd, request->file_requested, extension);
			return 0;
		}
	}
	return -1;
}

/**
 * Check, in mime type struct, if requested file type is supported.
 */
char *supported_type(char *file) {
	char *extension = NULL;
	char *ap = strrchr(file, '.');
	if(ap == NULL) {
		return "";
	}
	char *type = malloc(strlen(ap));
	sprintf(type, "%s", ap+1);
	int i;
	if(type == NULL)
		return "";
	for(i = 0; extensions[i].ext != NULL; i++) {
		if (strcmp(type, extensions[i].ext) == 0) {
			extension = malloc(strlen(extensions[i].mediatype));
			if(extension == NULL) {
				perror("Memory Allocation Failure\n");
				return NULL;
			} 
			extension = extensions[i].mediatype;
			return extension;
		}
	}
	return NULL;
}
