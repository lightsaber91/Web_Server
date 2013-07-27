#include "../lib/thread.h"

void create_thread(struct server_setting *setting, int socket, bool toLog, FILE *LogFile) {

	struct thread_job *job = malloc(sizeof(struct thread_job));
	if(job == NULL) {
		perror("Memory Allocation Failure\n");
		exit(EXIT_FAILURE);
	}
	job->s = setting;
	job->socket = socket;
	job->toLog = toLog;
	job->LogFile = LogFile;
	job->maxKeepAliveReq = setting->MaxKeepAliveReq;

	pthread_create(&job->tid, NULL, manage_connection, job);

}

void *manage_connection(void *p){

	bool firstReq = true;

	struct thread_job *job = (struct thread_job *)p;

        timeout.tv_sec = job->s->timeout;
        timeout.tv_usec = 0;

	config_socket(job->socket, job->s->KeepAlive);

	char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));
	if(in_request == NULL) {
		perror("Memory Allocation Failure\n");
		close(job->socket);
		pthread_exit(NULL);
	}

	while(read_request(job->socket, in_request, firstReq) == 1 && job->maxKeepAliveReq > 0) {

		job->maxKeepAliveReq--;

		struct browser_request *request;
		request = parse_browser_request(in_request);

		concatenation(request, job->s);

		if(job->s->log_lvl > 0) {
			writeConnectionLog(LogFile, request);
		}

		if(respond(job->socket, request, job->toLog, job->LogFile) == -1) {
			close(job->socket);
			free(request);
			//nthread--;
			free(in_request);
			pthread_exit(NULL);
		}
		free(request);
		if(job->s->KeepAlive == false || strcasecmp(request->connection_type, "keep-alive") == 0 ) {
			break;
		}

		ConfigKeepAliveTimeout(job->socket, job->s->KeepAliveTimeout);

		in_request = (char *)realloc(in_request, REQ_SIZE*sizeof(char));
		//Reinizializzo la memoria a zero
		bzero(in_request, REQ_SIZE*sizeof(char));

		firstReq = false;

	}
	free(in_request);
	close(job->socket);
	pthread_exit(NULL);
}
