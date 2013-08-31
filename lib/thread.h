pthread_mutex_t clisd_mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_job {
	pthread_t tid;
	struct server_setting *s;
	int socket;
	int maxKeepAliveReq;
	bool toLog;
	FILE *LogFile;
	xmlNode *start;
};

void *manage_connection(void *p);
