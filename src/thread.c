#include "../lib/thread.h"

void create_thread(struct server_setting *setting, int socket, bool toLog, FILE *LogFile) {

	struct thread_job *job = malloc(sizeof(struct thread_job));
	if(job == NULL) {
		perror("Memory Allocation Failure\n");
		//Non viene effettuata la exit() poichè altri thread potrebbero già essere in esecuzione
		//Annulliamo quindi solo la creazione del nuovo
		if(close(socket) == -1) {
			perror("Closing Socket\n");
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
			break;
		}

		if(read_request(job->socket, in_request, firstReq) != 1) {
			break; 
		}

		job->maxKeepAliveReq--;
		struct browser_request *request;
		request = parse_browser_request(in_request);
		if(request == NULL) {
			break;
		}
		concatenation(request, job->s);

		if(job->s->log_lvl > 0) {
			writeConnectionLog(LogFile, request);
		}

		if(respond(job->s, job->socket, request, job->toLog, job->LogFile) == -1) {
			break;
		}
		free(request);
		free(in_request);
		if(job->s->KeepAlive != true) {
			break;
		} 
		ConfigKeepAliveTimeout(job->socket, job->s->KeepAliveTimeout);
		firstReq = false;
	}
	if(close(job->socket) == -1) {
		perror("Closing Socket\n");
	}
	pthread_exit(NULL);
}
