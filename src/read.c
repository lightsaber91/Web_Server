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
 * Read request from the socket and save in buffer
 */
int read_request(int sockfd, char *buf, bool req) {
	int rec = 0, byteread = 0;
	do {
		rec = recv(sockfd, buf+byteread, REQ_SIZE-byteread, MSG_NOSIGNAL);
		if(strcmp(buf, "\r\n") != 0) {
			byteread += rec;
			if(byteread > 4) {
				if(buf[byteread-3] == '\n' && buf[byteread-4] == '\r') {
					return 1;
				}
			}
		}
		usleep(1);

	}while(rec > 0);
	if(req == true) {
		error_408(sockfd);
		if(toLog > -1)
			writeInfoLog(LOG_408, NULL);
	}
	return 0;	 
}

