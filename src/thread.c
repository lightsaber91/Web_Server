#include "../lib/thread.h"

void close_thread(int socket, char *in, struct browser_request *request) {

	if(shutdown(socket, SHUT_RDWR) == -1) {
		perror("Shutting down the socket\n");
	}
	if(in != NULL) {
		free(in);
	}
	if(request != NULL) {
		free(request);
	}
	pthread_exit(NULL);
}

void create_thread(struct server_setting *setting, int socket, bool toLog, FILE *LogFile) {

	struct thread_job *job = malloc(sizeof(struct thread_job));
	if(job == NULL) {
		perror("Memory Allocation Failure\n");
		//Non viene effettuata la exit() poichè altri thread potrebbero già essere in esecuzione
		//Annulliamo quindi solo la creazione del nuovo
		if(shutdown(socket, SHUT_RDWR) == -1) {
			perror("Shutting down the socket\n");
		}
		return;
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

	pthread_detach(pthread_self());
	//pthread_setschedprio(pthread_self(), HIGHPRIORITY);

        timeout.tv_sec = job->s->timeout;
        timeout.tv_usec = 0;

	config_socket(job->socket, job->s->KeepAlive);

	while(job->maxKeepAliveReq > 0) {

		char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));
		if(in_request == NULL) {
			close_thread(job->socket, NULL, NULL);
		}

		if(read_request(job->socket, in_request, firstReq) != 1)
			break; 

		job->maxKeepAliveReq--;

		struct browser_request *request;
		request = parse_browser_request(in_request);
		if(request == NULL) {
			close_thread(job->socket, in_request, NULL);
		}
		concatenation(request, job->s);

		if(job->s->log_lvl > 0) {
			writeConnectionLog(LogFile, request);
		}

		if(respond(job->socket, request, job->toLog, job->LogFile) == -1) {
			close_thread(job->socket, in_request, request);
		}
		free(request);
		free(in_request);
		if(job->s->KeepAlive == false) {
			break;
		}

		ConfigKeepAliveTimeout(job->socket, job->s->KeepAliveTimeout);
		firstReq = false;
	}
	shutdown(job->socket, SHUT_RDWR);
	pthread_exit(NULL);
}
