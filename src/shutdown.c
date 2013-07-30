#include "../lib/shutdown.h"

void shutdown_routine() {
	exit(EXIT_SUCCESS);
}

void sigterm_handler(int sig)
{
	if(sig == SIGTERM)
		printf("Signal SIGTERM recived: Shutting Down the Server\n");
	shutdown_routine();
}
