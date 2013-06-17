#include <pthread.h>

struct thread_job {
	pthread_t tid;
	struct server_setting *s;
	int socket;
	bool toLog;
	int LogFile;
}thread_job;

void *my_thread(void *p);
