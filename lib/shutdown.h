#include <signal.h>
/**
 * Teminate execution. 
 */
void shutdown_routine() {
	exit(EXIT_SUCCESS);
}
/**
 * Handler for SIGTERM signal, launch shutdown routine.
 */
void sigterm_handler(int sig)
{
	if(sig == SIGTERM)
		printf("Shutting Down the Server\n");
	shutdown_routine();
}
