#include <signal.h>
/**
 * Teminate execution. 
 */
void shutdown_routine() {
	//Write On Log When Server goes down
	if(toLog > -1)
		writeInfoLog(EXIT, NULL);
	exit(EXIT_SUCCESS);
}
/**
 * Handler for SIGTERM signal, launch shutdown routine.
 */
void sigterm_handler(int sig)
{
	if(sig == SIGTERM)
	shutdown_routine();
}
/**
 * Handler for SIGINT signal, launch shutdown routine.
 */
void sigint_handler(int sig)
{
	if(sig == SIGINT)
	shutdown_routine();
}
