#include "../lib/thread.h"

void *my_thread(void *p){

	bool firstReq = true;

	struct thread_job *job = (struct thread_job *)p;

        timeout.tv_sec = job->s->timeout;
        timeout.tv_usec = 0;

	config_socket(job->socket, job->s->KeepAlive);

	char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));

	while(read_request(job->socket, in_request, firstReq) == 1) {

		struct browser_request *request;
		request = parse_browser_request(in_request);
		printf("%s\n", request->user_agent );
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
		if(job->s->KeepAlive == true) {
			ConfigKeepAliveTimeout(job->socket, job->s->KeepAliveTimeout);
		}

		in_request = (char *)realloc(in_request, REQ_SIZE*sizeof(char));
		//Reinizializzo la memoria a zero
		bzero(in_request, REQ_SIZE*sizeof(char));

		firstReq = false;

	}
	free(in_request);
	close(job->socket);
	//nthread--;
	pthread_exit(NULL);
}
