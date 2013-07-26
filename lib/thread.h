#include <pthread.h>

struct thread_job {
	pthread_t tid;
	struct server_setting *s;
	int socket;
	bool toLog;
	FILE *LogFile;
}thread_job;

void *manage_connection(void *p);
