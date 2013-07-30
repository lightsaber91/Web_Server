#include <pthread.h>

#define HIGHPRIORITY 10

struct thread_job {
	pthread_t tid;
	struct server_setting *s;
	int socket;
	int maxKeepAliveReq;
	bool toLog;
	FILE *LogFile;
};

void *manage_connection(void *p);
