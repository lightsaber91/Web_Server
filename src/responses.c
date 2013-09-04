#include "../lib/responses.h"

/**
 * According to file length ,choose dimension of send buffer.
 */
int choose_buf_size(off_t len) {
	if(len/BUF_SIZE_0 < 2)
		return BUF_SIZE_0;
	if(len/BUF_SIZE_1 < 2)
		return BUF_SIZE_1;
	if(len/BUF_SIZE_2 < 2)
		return BUF_SIZE_2;
	if(len/BUF_SIZE_3 < 2)
		return BUF_SIZE_3;
	if(len/BUF_SIZE_4 < 2)
		return BUF_SIZE_4;
	if(len/BUF_SIZE_5 < 2)
		return BUF_SIZE_5;
	else
		return BUF_SIZE_6;
}

/**
 * Send error 505: Http Version not supported.
 */
void error_505(int sockfd) {

	if(send(sockfd, ERR_505, strlen(ERR_505), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet header err 505");
		return;
	}

	if(send(sockfd, PAG_505, strlen(PAG_505), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet page err 505");
		return;
	}
}

/**
 * Send Error 400: Bad Request.
 */
void error_400(int sockfd) {

	if(send(sockfd, ERR_400, strlen(ERR_400), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet header err 400");
		return;
	}

	if(send(sockfd, PAG_400, strlen(PAG_400), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet page err 400");
		return;
	}
	close(sockfd);
}

/**
 * Send Error 408: Request Time-out.
 */
void error_408(int sockfd) {

	if(send(sockfd, ERR_408, strlen(ERR_408), MSG_NOSIGNAL) < 0) {
		if(errno == EPIPE)
			return;
		perror("Sending Packet header err 408");
		return;
	}

	if(send(sockfd, PAG_408, strlen(PAG_408), MSG_NOSIGNAL) < 0) {
		if(errno == EPIPE)
			return;
		perror("Sending Packet page err 408");
		return;
	}
}

/**
 * Send Error 404: File not Found.
 */
void error_404(int sockfd) {

	if(send(sockfd, ERR_404, strlen(ERR_404), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet header err 404");
		return;
	}
	if(send(sockfd, PAG_404, strlen(PAG_404), MSG_NOSIGNAL) < 0 ) {
		perror("Sending Packet page err 404");
		return;
	}
}

/**
 * Send Error 415: Unsupported Media Type.
 */
void error_415(int sockfd) {

	if(send(sockfd, ERR_415, strlen(ERR_415), MSG_NOSIGNAL) < 0) {
		perror("Sending Packet header err 415");
		return;
	}
	if(send(sockfd, PAG_415, strlen(PAG_415), MSG_NOSIGNAL) < 0 ) {
		perror("Sending Packet page err 415");
		return;
	}
}

/**
 * Send HTTP header of file.
 */
void send_header(int sockfd, char *file, char *ext) {
	int fd = open(file, O_RDONLY, S_IREAD | S_IWRITE);
	if(fd == -1){
		perror("Opening File Requested\n");
		return;
	}
	struct stat s;
	fstat(fd, &s);
	char header[BUF_SIZE_0];
	if(sprintf(header,"HTTP/1.1 200 OK\nContent-Length: %lu\nConnection: close\nContent-Type: %s\n\n", s.st_size, ext) < 0) {
		perror("In sprintf: nothing written\n");
		close(fd);
		return;
	}
	if(send(sockfd, header, strlen(header), MSG_NOSIGNAL) == -1) {
		perror("Sending Packet header in HEAD");
		close(fd);
		return;
	}
	close(fd);
}

/**
 * General function that send every file supported by the server.
 */
int send_file(int sockfd, char *file, char *ext) {
	int fd = open(file, O_RDONLY, S_IREAD | S_IWRITE);
	if(fd == -1){
		perror("Opening File Requested\n");
		return -1;
	}
	struct stat s;
	fstat(fd, &s);
	int ret = 0;
	int size = choose_buf_size(s.st_size);
	char header[size];
	if(sprintf(header,"HTTP/1.1 200 OK\nContent-Length: %lu\nConnection: Keep-Alive\nContent-Type: %s\n\n", s.st_size, ext) < 0) {
		perror("In sprintf: nothing written\n");
		close(fd);
		return -1;
	}
	if(send(sockfd, header, strlen(header), MSG_NOSIGNAL) == -1) {
		perror("Sending Packet Header");
		close(fd);
		return -1;
	}
	while ((ret = read(fd, header, size)) > 0 ){
		if(send(sockfd, header, ret, MSG_NOSIGNAL) == -1) {
			perror("Sending Packet");
			close(fd);
			return -1;
		}
	}
	close(fd);
	return 0;
}

/**
 * This function resize image if enabled in setting, than call send_file function,
 * otherwise call send_file on image without resizing it.
 */
int send_image(SETTING *s, int sockfd, char *file, char *ext, char *user_agent, int quality) {

	if(s->use_wurfl == true) {
		DEVICE *property = malloc(sizeof(DEVICE));
		if(property != NULL) { 
			if(parse_UA(user_agent, property, s->start) == 0) {
				char *file_resized = cache_by_resolution(property->resolution_width, property->resolution_height, file);
				return send_file(sockfd, file_resized, ext);
			}
			else {
				char *lowq_file = cache_by_quality(file, quality); 
				return send_file(sockfd, lowq_file, ext);
			}
		}
		else {
			char *lowq_file = cache_by_quality(file, quality); 
			return send_file(sockfd, lowq_file, ext);
		}
	}
	else { 
		return send_file(sockfd, file, ext);
	}
	return -1;
}

/**
 * Analize http field in parsed request, in case of errors send the correct error message,
 * in case of HEADER request send only the header and in case of GET send the file.
 * If error occurs writes in the log file. 
 */
int respond(SETTING *s, int sockfd, HTTP_CONN *request, char *path_to_file) {

	if(strcasecmp(request->http_version, "HTTP/1.0") != 0 && strcasecmp(request->http_version, "HTTP/1.1") != 0) {
		error_505(sockfd);
		if(toLog > 0) {
			writeInfoLog(LOG_505, NULL);
		}
		return -1;
	}

	else if(strcasecmp(request->method, "GET") != 0 && strcasecmp(request->method, "HEAD") != 0) {
		error_400(sockfd);
		if(toLog > 0) {
			writeInfoLog(LOG_400, NULL);
		}
		return -1;
	}

	else {
		char *extension = supported_type(path_to_file);

		if(extension == NULL) {
			error_415(sockfd);
			if(toLog > 0) {
				writeInfoLog(LOG_415, NULL);
			}
			return -1;
		}

		else if(access(path_to_file, F_OK) == -1) {
			error_404(sockfd);
			if(toLog > 0) {
				writeInfoLog(LOG_404, request);
			}
			return 0;
		}

		else if(strcasecmp(request->method, "HEAD") == 0) {
			send_header(sockfd, path_to_file, extension);
			return 0;
		}

		else {
			if(strncmp(extension, "image", 5) == 0) {
				return send_image(s, sockfd, path_to_file, extension, request->user_agent, request->accept_quality);
			}
			else 
				return send_file(sockfd, path_to_file, extension);
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
