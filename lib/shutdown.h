/**
 * Web Server
 * Copyright (C) 2013 - Simone Martucci <martucci.simone.91@gmail.com>
 *
 *
 * This Program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
**/

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
