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

#include "../lib/thread.h"

/**
 * This function create structs and save there everything threads need,
 * than launch new thread with pthread_create.
 */
void create_thread(SETTING *setting, int socket) {

	T_INFO *job = malloc(sizeof(T_INFO));
	if(job == NULL) {
		perror("Memory Allocation Failure\n");
		//Non viene effettuata la exit() poichè altri thread potrebbero già essere in esecuzione
		//Annulliamo quindi solo la creazione del nuovo
		if(close(socket) == -1) {
			perror("Closing Socket\n");
		}
		return;
	}
	job->s = setting;
	job->socket = socket;
	job->maxKeepAliveReq = setting->MaxKeepAliveReq;

	pthread_create(&job->tid, NULL, manage_connection, job);
}

/**
 * The web server core is here. This is how thread works.
 * After read from socket the thread respond, even with errors, and/or close connection, than terminate its execution.
 */
void *manage_connection(void *p){

	bool firstReq = true;

	T_INFO *job = (T_INFO *)p;
	//pthread_detach is used because when a thread terminate all its memory will be automatically released.
	pthread_detach(pthread_self());
	//Set socket timeout.
        timeout.tv_sec = job->s->timeout;
        timeout.tv_usec = 0;
	config_socket(job->socket, job->s->KeepAlive);

	//Write On Log creation of new threads
	if(toLog > -1)
		writeInfoLog(T_CREAT, NULL);

	while(job->maxKeepAliveReq > 0) {

		char *in_request = (char *)malloc(REQ_SIZE*sizeof(char));
		if(in_request == NULL) {
			break;
		}

		if(read_request(job->socket, in_request, firstReq) != 1) {
			break; 
		}
		job->maxKeepAliveReq--;

		HTTP_CONN *request;

		pthread_mutex_lock(&clisd_mutex);
		request = parse_client_request(in_request);
		pthread_mutex_unlock(&clisd_mutex);

		if(request == NULL) {
			break;
		}
		//real location of resource requested from client in the server memory
		char *path_to_file = concatenation(request, job->s);

		if(toLog == 1) {
			writeConnectionLog(request);
		}
		if(respond(job->s, job->socket, request, path_to_file) == -1) {
			free(request);
			free(in_request);
			break;
		}
		free(request);
		free(in_request);
		if(job->s->KeepAlive != true) {
			break;
		} 
		ConfigKeepAliveTimeout(job->socket, job->s->KeepAliveTimeout);
		firstReq = false;
	}
	n_thread--;
	if(close(job->socket) == -1) {
		perror("Closing Socket\n");
	}
	//Write on Log when a thread terminates
	if(toLog > -1)
		writeInfoLog(T_EXIT, NULL);
	pthread_exit(NULL);
}
