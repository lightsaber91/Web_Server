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

/**
 * Merge page request from browser and browser's root folder
 * to obtain correct path of phisical file in the server
 */
char *concatenation (HTTP_CONN *request, SETTING *setting) {

	if(strcmp(request->file_requested, "/") == 0) {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(setting->home_page)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, setting->home_page) < 0) {
			perror("In sprintf: nothing written\n");
		}
		return rootFile;
	}
	else {
		char *rootFile = malloc(strlen(setting->root_folder)+strlen(request->file_requested)+1);
		if(rootFile == NULL) {
			perror("Memory Allocation Failure\n");
		}
		if(sprintf(rootFile, "%s%s", setting->root_folder, request->file_requested) < 0) {
			perror("In sprintf: nothing written\n");
		}
		return rootFile;
	}

}


/**
 * Set socketoption like keepalive, sendbuffer dimension, and keep alive timeout
 */
void config_socket(int sockfd, bool KeepAlive) {
	int reuse = 1;
	int sendbuff = 65536;
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff)) < 0) {
		perror("In SetSockOpt\n");
	}
	if(setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &reuse, sizeof(reuse)) < 0) {
		perror("In SetSockOpt\n");
	}

	if(KeepAlive == true) {
		if(setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &reuse, sizeof(reuse)) < 0) {
			perror("In SetSockOpt\n");
		}
	}
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }
	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
                perror("In SetSockOpt\n");
        }

}

