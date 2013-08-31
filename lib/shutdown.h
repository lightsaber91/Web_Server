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
		printf("\nShutting Down the Server\n");
	shutdown_routine();
}
/**
 * Handler for SIGINT signal, launch shutdown routine.
 */
void sigint_handler(int sig)
{
	if(sig == SIGINT)
		printf("\nShutting Down the Server\n");
	shutdown_routine();
}
