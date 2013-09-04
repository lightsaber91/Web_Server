#define T_CREAT "Thread Created"
#define T_EXIT "Thread Terminated"

typedef struct thread_job {
	pthread_t tid;
	SETTING *s;
	int socket;
	int maxKeepAliveReq;
	xmlNode *start;
}T_INFO;

void *manage_connection(void *p);
